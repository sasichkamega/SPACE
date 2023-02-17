#include <spdlog/spdlog.h>
#include "config2.hpp"

namespace engine::config2 {

  namespace detail {
    /*void UpdateCvar(flecs::iter it, const ConfigVar* config_var, const Validator* validator) {
      flecs::world world = it.world();
      world.remove_all<UpdateData>();
      for (auto i : it) {
        flecs::entity e = it.entity(i);
        bool validate_result = true;
        if (validator) {
          if (!validator[i].func) {
            SPDLOG_ERROR("Validator has no func. Entity: {}", e.path("::", ""));
            validate_result = false;
          } else {
            validate_result = validator[i].func(e);
          }
        }

        SPDLOG_CRITICAL("BOP");
        e.set_second<ValidatedData>(config_var->data_component);

        if (validate_result == true) {

        } else {

        }
      }
    }*/
  };

  cvar& cvar::cvar_validate(std::function<bool(flecs::entity)>&& func) {
    if (this->has<Validator>()) {
      SPDLOG_WARN("Entity '{}' already has validator. It will be overwritten.", this->path("::", ""));
    }
    this->set<Validator>({.func = std::move(func)});
    return *this;
  }

} //namespace engine::config2

engine::Config2::Config2(flecs::world& world) {
  using namespace config2;
  world.module<Config2>("config2");

  //components
  world.component<ValidatedData>();
  world.component<UnValidatedData>();
  world.component<DefaultData>();
  world.component<Validator>();
  world.component<UpdateData>();
  world.component<OnUpdate>();
  world.component<SystemsInitialised>();
  world.component<ConfigVar>()
    .member(flecs::Entity, "data_component");
  world.component<type::Int32>("Int32")
    .member<decltype(type::Int32::value)>("value");
  world.component<type::Uint32>("Uint32")
    .member<decltype(type::Uint32::value)>("value");

  //phases
  this->validate_phase = world.entity("phase::validate").add(flecs::Phase).depends_on(flecs::PostFrame);
  this->update_phase = world.entity("phase::update").add(flecs::Phase).depends_on(validate_phase);
  this->notify_phase = world.entity("phase::notify").add(flecs::Phase).depends_on(update_phase);

  
  /*world.system<const ConfigVar, const Validator>("system::UpdateCvar")
    .kind(flecs::PostFrame)
    .arg(2).optional()
    .with<UpdateData>()
    .iter(detail::UpdateCvar);*/


};
