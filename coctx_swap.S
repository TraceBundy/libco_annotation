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

.globl coctx_swap
#if !defined( __APPLE__ )
.type  coctx_swap, @function
#endif
coctx_swap:

#if defined(__i386__)
	leal 4(%esp), %eax //sp  //指向第一个参数
	movl 4(%esp), %esp // 执行curr coctx_t regs[0] 
	leal 32(%esp), %esp //parm a : &regs[7] + sizeof(void*) //32位有8个寄存器 只有regs[7]

	pushl %eax //esp ->parm a //压栈

	pushl %ebp
	pushl %esi
	pushl %edi
	pushl %edx
	pushl %ecx
	pushl %ebx
	pushl -4(%eax)

	
	movl 4(%eax), %esp //parm b -> &regs[0] //切换到pending_co 

	popl %eax  //ret func addr // //暂存返回地址
	popl %ebx  
	popl %ecx
	popl %edx
	popl %edi
	popl %esi
	popl %ebp
	popl %esp
	pushl %eax //set ret func addr //返回地址压栈

	xorl %eax, %eax //清零
	ret

#elif defined(__x86_64__)
	leaq 8(%rsp),%rax //curr 第一个参数 
	leaq 112(%rdi),%rsp // 64位 16个寄存器 //指向reg[15]
	pushq %rax //将参数压栈  这个地址是curr的堆
	pushq %rbx
	pushq %rcx
	pushq %rdx

	pushq -8(%rax) //ret func addr //压入返回返回地址

	pushq %rsi
	pushq %rdi
	pushq %rbp
	pushq %r8
	pushq %r9
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	
	movq %rsi, %rsp //切换到pending_co
	popq %r15 //将pending_co 堆内参数设置到寄存器 
	popq %r14
	popq %r13
	popq %r12
	popq %r9
	popq %r8
	popq %rbp
	popq %rdi
	popq %rsi
	popq %rax //ret func addr
	popq %rdx
	popq %rcx
	popq %rbx
	popq %rsp
	pushq %rax
    //清空eax	
	xorl %eax, %eax
	ret
#endif
