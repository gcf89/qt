#pragma once

#include <QSharedPointer>

#define DefineClassS(type) class type; typedef QSharedPointer<type> type##S; typedef QWeakPointer<type> type##W
