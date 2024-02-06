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
    
    //����ϴ� �޽��� RectMesh(���� �����̽��� ������ 0.5¥�� ���簢��)
    //���� 2��� Ű���� ȭ�� ��ü�� �ȼ����̴��� ȣ��� �� �ְ� �Ѵ�
    output.vPosition = float4(_in.vPos.xyz * 2.f, 1.f);
    
    return output;
}

float4 PS_MiddleMergeShader(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);
    
    //�� �ȼ��� ȣ��� ��ġ���� �� ���
    float2 vScreenUV = _in.vPosition.xy / g_Resolution.xy;

    
    float4 velocity = VelocityTargetTex.Sample(g_sam_0, vScreenUV);
   
    
    const int SAMPLE_COUNT = 10; // ���ø� Ƚ��
       
    velocity.xy /= SAMPLE_COUNT;
    
    int iCOUNT = 1;
    float4 vColor;
    float4 vBackColor;
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        float2 sampleTexCoord = vScreenUV + (velocity.xy * float(i) * 0.04f);
        vBackColor = ColorTargetTex.Sample(g_sam_0, sampleTexCoord.xy);
        
        vColor += vBackColor;
        ++iCOUNT;
    }
    
    vColor /= (float) iCOUNT;
    
    vOutColor.xyz = vColor.xyz;
    
 
    return vOutColor;
}


#endif