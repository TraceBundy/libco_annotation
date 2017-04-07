/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#ifndef __CO_ROUTINE_INNER_H__

#include "co_routine.h"
#include "coctx.h"
struct stCoRoutineEnv_t;
struct stCoSpec_t
{
	void *value;
};

struct stStackMem_t
{
	stCoRoutine_t* ocupy_co;
	int stack_size;
	char* stack_bp; //stack_buffer + stack_size
	char* stack_buffer;

};

struct stShareStack_t
{
	unsigned int alloc_idx;
	int stack_size;
	int count;
	stStackMem_t** stack_array;
};



struct stCoRoutine_t
{
	stCoRoutineEnv_t *env; //当前线程的环境
	pfn_co_routine_t pfn; //回调函数
	void *arg; //回调参数
	coctx_t ctx; //协程切换的上下文信息

	char cStart; //resume 时为 1
	char cEnd; //release 为1
	char cIsMain; //是否是主线程环境里的 routine 在设置线程私有数据时用到
	char cEnableSysHook; //设置协程是否系统函数被钩住
	char cIsShareStack; //是否使用共享栈空间

	void *pvEnv;

	//char sRunStack[ 1024 * 128 ];
	stStackMem_t* stack_mem; //非共享栈空间时候使用


	//save satck buffer while confilct on same stack_buffer;
	char* stack_sp; //保存栈顶， 在co_swap 来使用， 用来找到调用函数的地址
    //在共享栈空间时候， 会用到保存的栈信息
	unsigned int save_size;
	char* save_buffer;

    //线程私有数据
	stCoSpec_t aSpec[1024];

};


//初始线程环境
//1.env
void 				co_init_curr_thread_env();
stCoRoutineEnv_t *	co_get_curr_thread_env();

//2.coroutine
void    co_free( stCoRoutine_t * co ); //释放routine
void    co_yield_env(  stCoRoutineEnv_t *env ); //当前协程yield

//3.func



//-----------------------------------------------------------------------------------------------
//超时队列
struct stTimeout_t;
struct stTimeoutItem_t ;

stTimeout_t *AllocTimeout( int iSize );
void 	FreeTimeout( stTimeout_t *apTimeout );
int  	AddTimeout( stTimeout_t *apTimeout,stTimeoutItem_t *apItem ,uint64_t allNow );

struct stCoEpoll_t;
stCoEpoll_t * AllocEpoll();
void 		FreeEpoll( stCoEpoll_t *ctx );

stCoRoutine_t *		GetCurrThreadCo();
void 				SetEpoll( stCoRoutineEnv_t *env,stCoEpoll_t *ev );

typedef void (*pfnCoRoutineFunc_t)();

#endif

#define __CO_ROUTINE_INNER_H__
