// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2023 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#ifndef PX_FORCE_MODE_H
#define PX_FORCE_MODE_H

#include "foundation/PxPreprocessor.h"

/** \addtogroup physics
@{
*/

#if !PX_DOXYGEN
namespace physx
{
#endif

/**
\brief Parameter to addForce() and addTorque() calls, determines the exact operation that is carried out.

@see PxRigidBody.addForce() PxRigidBody.addTorque()
*/
struct PxForceMode
{
	/*parameter는 질량* 길이 / 시간 ^ 2의 단위, 즉 힘을 갖습니다
	parameter는 질량* 길이 / 시간 단위, 즉 힘* 시간 단위를 갖습니다
	매개변수는 길이 / 시간의 단위를 갖습니다.즉, 효과는 질량에 독립적입니다.즉, 속도 변화입니다.
	매개변수는 길이 / 시간 ^ 2의 단위, 즉 가속도를 갖습니다.적분하기 전에 질량을 나누지 않는 것을 제외하고는 힘과 같이 취급됩니다.*/

	enum Enum
	{
		eFORCE,				//!< parameter has unit of mass * length / time^2, i.e., a force
		eIMPULSE,			//!< parameter has unit of mass * length / time, i.e., force * time
		eVELOCITY_CHANGE,	//!< parameter has unit of length / time, i.e., the effect is mass independent: a velocity change.
		eACCELERATION		//!< parameter has unit of length/ time^2, i.e., an acceleration. It gets treated just like a force except the mass is not divided out before integration.
	};
};

#if !PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
