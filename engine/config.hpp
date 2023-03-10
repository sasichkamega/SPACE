#pragma once
#include <string>
#include <string_view>
#include <functional>
#include <variant>
#include <flecs.h>

namespace engine {

struct Config {
  Config(flecs::world&);
};

} //namespace engine

namespace engine::config {

  namespace detail {
    //добавим в тип варианта для того, что бы meta модуль flecs не считал 
    //за один тип наш VariantType и такой же std::varint<string, int32, float>
    struct VariantUniqueTag {
      //Для сравнений
      inline operator bool() const noexcept {
        return true;
      }
    };
  } //namespace detail;

  using VariantType = std::variant<std::string, std::int32_t, float, detail::VariantUniqueTag>;
  std::string_view stored_typename(const VariantType& data) noexcept;

  namespace detail {    
    //internal, used in create_typed_var_listener
    void print_error_report(const std::string_view expected_type,
                            const VariantType& variant,
                            const std::string_view var_name,
                            const std::string_view listener_name,
                            flecs::world& world);
  }

struct Var {
  VariantType data;
};

struct Changed {};
struct ChangeListener {
  using CbType = std::function<void(flecs::world&, const VariantType&)>;
  CbType cb;
};

void create_var_listener(flecs::world& world,
                         const std::string_view var_name,
                         const std::string_view listener_name,
                         ChangeListener::CbType&& onchange_cb);

inline void create_var_listener(flecs::world& world,
                         const std::string_view entity_name,
                         ChangeListener::CbType&& onchange_cb)
{
  create_var_listener(world, entity_name, entity_name, std::move(onchange_cb));
}

template <class T>
using TypedChangeLisneterCb = std::function<void (flecs::world&, const T&)>;

template <class T>
void create_typed_var_listener(flecs::world& world,
                         const std::string_view var_name,
                         const std::string_view listener_name,
                         TypedChangeLisneterCb<T>&& onchange_cb)
{
  using namespace std::string_view_literals;

  ChangeListener::CbType cb = [typed_cb = std::move(onchange_cb), var_name, listener_name]
                              (flecs::world& world, const VariantType& variant)
  {
    if (const T* value = std::get_if<T>(&variant); value) {
      typed_cb(world, *value);
    } else {
      detail::print_error_report("IMPLEMENT ME"sv, variant, var_name, listener_name, world);
    }
  };
  create_var_listener(world, var_name, listener_name, std::move(cb));
}

void create_var(flecs::world& world, const std::string_view name, VariantType&& data);
void update_var(flecs::world& world, const std::string_view name, VariantType&& data);
//return nullptr if var not exits
const VariantType* get_var(flecs::world& world, const std::string_view name);

template<class T>
inline const T* get_var(flecs::world& world, const std::string_view name) {
  const VariantType* data = get_var(world, name);
  if (!data) {
    return nullptr;
  }
  if (const T* value = std::get_if<T>(data); value) {
    return value;
  }
  return nullptr;
}

} //namespace engine::config