/*
 *  Scion - an open-source implementation of the Maxis GZCOM/RZCOM framework
 *  Copyright (C) 2021  Nelson Gomez (nsgomez) <nelson@ngomez.me>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation, under
 *  version 2.1 of the License.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once
#include <stdint.h>

typedef uint32_t GZCLSID;
typedef uint32_t GZGUID;
typedef uint32_t GZIID;

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2)  CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2) CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2) arg1##arg2

#define FOR_EACH_1(what, x, ...) what(x)
#define FOR_EACH_2(what, x, ...) what(x) FOR_EACH_1(what, __VA_ARGS__)
#define FOR_EACH_3(what, x, ...) what(x) FOR_EACH_2(what, __VA_ARGS__)
#define FOR_EACH_4(what, x, ...) what(x) FOR_EACH_3(what, __VA_ARGS__)
#define FOR_EACH_5(what, x, ...) what(x) FOR_EACH_4(what, __VA_ARGS__)
#define FOR_EACH_6(what, x, ...) what(x) FOR_EACH_5(what, __VA_ARGS__)
#define FOR_EACH_7(what, x, ...) what(x) FOR_EACH_6(what, __VA_ARGS__)
#define FOR_EACH_8(what, x, ...)                                               \
    what(x);                                                                   \
    FOR_EACH_7(what, __VA_ARGS__)

#define FOR_EACH_NARG(...)                                     FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...)                                    FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define FOR_EACH_RSEQ_N()                                      8, 7, 6, 5, 4, 3, 2, 1

#define FOR_EACH_(N, what, x, ...)                                             \
    CONCATENATE(FOR_EACH_, N)(what, x, __VA_ARGS__)
#define FOR_EACH(what, x, ...)                                                 \
    FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, x, __VA_ARGS__)

#define FOURCC(a, b, c, d)                                                     \
    ((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c) << 16) |            \
     ((uint32_t)(d) << 24))

#define MAKE_FOURCC(fourbyte)                                                   \
    FOURCC(fourbyte[0], fourbyte[1], fourbyte[2], fourbyte[3])