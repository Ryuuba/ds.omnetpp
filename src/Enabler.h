//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#if !defined(ENABLER_H)
#define ENABLER_H

#include "Status.h"
#include "Event.h"

class Enabler {
public:
  Status status;
  EventKind event;
  Enabler(const Status& status, EventKind event) {
    this->status = status;
    this->event = event;
  }
  void set(const Status& status, EventKind event) {
    this->status = status;
    this->event = event;
  }
  bool operator==(const Enabler& e) {
    return (this->status == e.status) && (this->event == event);
  }
  bool operator==(const Enabler& e) const {
    return (this->status == e.status) && (this->event == event);
  }
};


struct EnablerHasher {
  std::size_t operator()(Enabler const& e) const noexcept {
    size_t hash = std::hash<int>{}((e.status.get() + e.event)*(e.status.get() + e.event + 1)/2 + e.event);
    return hash;
  }
};


#endif // ENABLER_H
