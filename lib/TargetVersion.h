#pragma once
#ifndef TARGETVERSION_H
#define TARGETVERSION_H

#define MAKE_STRING(x) #x
#define EXPAND_STRING(x) MAKE_STRING(x)

#define TARGET_VERSION_STRING EXPAND_STRING( TARGET_VERSION )

#endif //TARGETVERSION_H