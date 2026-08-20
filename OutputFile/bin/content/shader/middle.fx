#ifndef _LIGHT
#define _LIGHT

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
};

// =====================================
// MergeShader
// MRT              : SwapChain
// Domain           : DOMAIN_LIGHT
// Mesh             : RectMesh
// Rasterizer       : CULL_BACK
// DepthStencil     : NO_TEST_NO_WRITE
// Blend            : Default

//Parameter
#define ColorTargetTex  g_tex_0
#define VelocityTargetTex g_tex_1


VS_OUT VS_MiddleMergeShader(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    //사용하는 메쉬가 RectMesh(로컬 스페이스에 반지름 0.5짜리 정사각형)
    //따라서 2배로 키워서 화면 전체가 픽셀쉐이더가 호출될 수 있게 한다
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    
    return output;
}

float4 PS_MiddleMergeShader(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    float2 vScreenUV = _in.vPosition.xy / g_Resolution.xy;

    float4 velocity = VelocityTargetTex.Sample(g_sam_0, vScreenUV);

    const int SAMPLE_COUNT = 10;
    velocity.xy /= SAMPLE_COUNT;

    float4 vColor = (float4) 0.f;
    float4 vBackColor;
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        float2 sampleTexCoord = vScreenUV + (velocity.xy * float(i) * 0.04f);
        vBackColor = ColorTargetTex.Sample(g_sam_0, sampleTexCoord.xy);

        vColor += vBackColor;
    }

    vColor /= (float) SAMPLE_COUNT;

    vOutColor.xyz = vColor.xyz;

    return vOutColor;
}


#endif