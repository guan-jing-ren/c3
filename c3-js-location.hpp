#ifndef C3_JS_LOCATION_HPP
#define C3_JS_LOCATION_HPP

#include "c3-js-fwd.hpp"

struct Location {
  val location = val::null();

  DOMString href() const { return location["href"].as<DOMString>(); }
  void href(const DOMString &value) { return location.set("href", value); }
  DOMString protocol() const { return location["protocol"].as<DOMString>(); }
  void protocol(const DOMString &value) {
    return location.set("protocol", value);
  }
  DOMString host() const { return location["host"].as<DOMString>(); }
  void host(const DOMString &value) { return location.set("host", value); }
  DOMString hostname() const { return location["hostname"].as<DOMString>(); }
  void hostname(const DOMString &value) {
    return location.set("hostname", value);
  }
  DOMString port() const { return location["port"].as<DOMString>(); }
  void port(const DOMString &value) { return location.set("port", value); }
  DOMString pathname() const { return location["pathname"].as<DOMString>(); }
  void pathname(const DOMString &value) {
    return location.set("pathname", value);
  }
  DOMString search() const { return location["search"].as<DOMString>(); }
  void search(const DOMString &value) { return location.set("search", value); }
  DOMString hash() const { return location["hash"].as<DOMString>(); }
  void hash(const DOMString &value) { return location.set("hash", value); }
  DOMString username() const { return location["username"].as<DOMString>(); }
  void username(const DOMString &value) {
    return location.set("username", value);
  }
  DOMString password() const { return location["password"].as<DOMString>(); }
  void password(const DOMString &value) {
    return location.set("password", value);
  }
  DOMString origin() const { return location["origin"].as<DOMString>(); }

  void assign(const DOMString &url) {
    return location.call<void>("assign", val(url));
  }
  void reload(bool force = false) {
    return location.call<void>("reload", val(force));
  }
  void replace(const DOMString &url) {
    return location.call<void>("replace", val(url));
  }
  DOMString toString() const { return location.call<DOMString>("toString"); }
};

#endif
