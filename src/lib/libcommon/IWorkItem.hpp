/** \brief Work Item interface
 *
 *  Inherit from this class if you wish work in
 *  your class to be serviced by a generic WorkQueue.
 *  It is not necessary to provide any implementations;
 *  queue up your void function pointers using the
 *  enqueue function. Then queue your class up on
 *  a WorkQueue, and wait for the job to spool.
 *
 * NOTE: It is up to you to keep your IWorkItem in
 *       scope until either the job spools or the
 *       WorkQueue is stopped.
 */
#pragma once
#include <functional>

namespace common
{

class WorkQueue;

class IWorkItem
{
public:
  virtual ~IWorkItem() {}
  virtual void doWork() = 0;
  virtual bool enqueue(std::function<void(void)> f) = 0;
}; // class IWorkItem

}; // namespace common

