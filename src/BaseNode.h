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

#if !defined(BASENODE_H)
#define BASENODE_H

#include <omnetpp.h>
#include <vector>
#include <unordered_map>
#include <functional>

#include "Enabler.h"

using omnetpp::cMessage;

class BaseNode : public omnetpp::cSimpleModule {
protected:
  /** @brief An event to active this node spontaneously */
  Event spontaneously;
  /** @brief The current status of this node */
  Status status;
  /** @brief The set of rules this node obeys. The structure of a rule is:
   *  (status, event) -> action. The elements of the pair (status, event) 
   *  corresponds to objects of kind Status and MsgKind, properly. Note that
   *  both the spotaneous impulse and the ringing of a timer are produced by 
   *  self-messages, thus the second element (the event) is a message. The 
   *  action is a member-function that the user of this class must register in 
   *  the initialize method as follows:
   * 
   *  rule[enabler] = &action,
   * 
   *  where the enable is an object of kind Enabler and action is a BaseNode 
   *  member-function with signature:
   * 
   *  void BaseNode::action(omnetpp::cMessage*) 
   */
  std::unordered_map<
    Enabler, 
    std::function<void(const BaseNode&, Event)>,
    EnablerHasher
  > rule;
protected:
  /** @brief Broadcasts a message to N(x) 
   *  @param first - a valid pointer to a message
   *  @return a null pointer to the received message
  */
  omnetpp::cMessage* localBroadcast(omnetpp::cMessage*);
  /** @brief Broadcasts a message to N(x) - {senderID}
   *  @param first - a valid pointer to a message
   *  @param second - the ID of the sender
   *  @return a null pointer to the received message
  */
  omnetpp::cMessage* localFlooding(omnetpp::cMessage*);
  /** @brief Multicasts a message to a subset of N(x)
   *  @param first - a valid pointer to a message
   *  @param second - a vector holding the IDs of receivers
   *  @return a null pointer to the received message
  */
  omnetpp::cMessage* localMulticast(
    omnetpp::cMessage*, const std::vector<int>&
  );
  /** @brief Displays a string in the simulation canvas */
  virtual void displayInfo(const char* info) const{
    getDisplayString().setTagArg("t", 0, info);
  }
  /** @brief Changes the color of the info string by a standard HTML color */
  virtual void changeInfoColor(const char* color) const{
    getDisplayString().setTagArg("t", 2, color);
  }
  /** @brief A warning message stating a node executes nil */
  virtual void nil(omnetpp::cMessage* msg) {
    EV_WARN << "Undefinded action, assuming (" << status.str() << ", " 
            << msg->getName() << ") -> nil" << '\n';
  }
  /** @brief Changes width of an edge 
   *  @param first - The number of port to access the link
   *  @param second - The width of the edge
  */
  virtual void changeEdgeWidth(int, int);
  /** @brief Changes the color of an edge 
   *  @param first - The number of port to access the link
   *  @param second - The name of a HTML standard color
  */
  virtual void changeEdgeColor(int, const char*);
public:
  /** @brief Default constructor */
  BaseNode() : spontaneously(nullptr), status() { }
  /** @brief Default destructor which tries to delete 
   *  the event "spontaneously" */
  virtual ~BaseNode() { cancelAndDelete(spontaneously); }
  /** @brief Sets the initial status of protocols according to its role. In 
   *  addition, records the rules this node obeys.
   */
  virtual void initialize() = 0;
  /** @brief Invokes the action corresponding to a given (status, event) pair.
   *  If the action is undefined, then nil is invoke.
   */
  virtual void handleMessage(omnetpp::cMessage*);
};

#endif // BASENODE_H
