/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#include "ExecFlagEnum.h"
#include "MooseError.h"
#include "Conversion.h"

ExecFlagEnum::ExecFlagEnum() : MultiMooseEnum() {}
ExecFlagEnum::ExecFlagEnum(const MultiMooseEnum & other) : MultiMooseEnum(other) {}
ExecFlagEnum::ExecFlagEnum(const ExecFlagEnum & other) : MultiMooseEnum(other) {}

void
ExecFlagEnum::addAvailableFlags(const ExecFlagType & flag)
{
  if (flag.id() == MooseEnumItem::INVALID_ID)
  {
    // It is desired that users when creating ExecFlagTypes should not worry about needing
    // to assign a name and an ID. However, the ExecFlagTypes created by users are global constants
    // and the ID to be assigned can't be known at construction time of this global constant, it is
    // only known when it is added to this object (ExecFlagEnum). Therefore, this const cast allows
    // the ID to be set after construction. This was the lesser of two evils: const_cast or
    // friend class with mutable members.
    ExecFlagType & non_const_flag = const_cast<ExecFlagType &>(flag);
    non_const_flag.setID(Moose::execute_flags.getNextValidID());
  }
  addEnumerationItem(flag);
}

void
ExecFlagEnum::removeAvailableFlags(const ExecFlagType & flag)
{
  if (find(flag) == _items.end())
    mooseError("The supplied item '",
               flag,
               "' is not an available enum item for the "
               "MultiMooseEnum object, thus it cannot be removed.");
  else if (contains(flag))
    mooseError("The supplied item '", flag, "' is a selected item, thus it can not be removed.");

  _items.erase(flag);
}

std::string
ExecFlagEnum::getDocString() const
{
  std::string doc("The list of flag(s) indicating when this object should be executed, the "
                  "available options include ");
  doc += Moose::stringify(getNames(), ", ");
  doc += ".";
  return doc;
}

ExecFlagEnum &
ExecFlagEnum::operator=(const std::initializer_list<ExecFlagType> & flags)
{
  clear();
  *this += flags;
  return *this;
}

ExecFlagEnum &
ExecFlagEnum::operator=(const ExecFlagType & flag)
{
  clear();
  *this += flag;
  return *this;
}

ExecFlagEnum &
ExecFlagEnum::operator+=(const std::initializer_list<ExecFlagType> & flags)
{
  for (const ExecFlagType & flag : flags)
    appendCurrent(flag);
  checkDeprecated();
  return *this;
}

ExecFlagEnum &
ExecFlagEnum::operator+=(const ExecFlagType & flag)
{
  appendCurrent(flag);
  checkDeprecated();
  return *this;
}

void
ExecFlagEnum::appendCurrent(const ExecFlagType & item)
{
  if (find(item) == _items.end())
    mooseError("The supplied item '",
               item,
               "' is not an available item for the "
               "ExecFlagEnum object, thus it cannot be set as current.");
  _current.push_back(item);
}
