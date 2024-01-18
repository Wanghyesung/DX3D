#ifndef _PARTICLE_RENDER
#define _PARTICLE_RENDER

#include "value.fx"
#include "struct.fx"

struct VS_MOTIONBLUR_IN
{
    float3 vPos : POSITION;
    
    //float3 vPrevPos : PREVPOSITIONT;
    
    float2 vUV : TEXCOORD;
    
    float3 vNormal : NORMAL;
};

struct VS_MOTIONBLUR_OUT
{
    float4 vPosition : SV_Position;
    
    float4 vVelocity : VELOCITY;
    
    float2 vUV : TEXCOORD;
 
    float3 vNormal : NORMAL;
};

struct PS_MOTIONLUR_OUT
{
    float4 vVelocity : SV_Target0;
    float4 vData : SV_Target1;
};


// ===============
// MotionBlur
// DOMAIN : Motionblur
// MRT    : MotionBlur MRT
// Rasterizer State     : CULL_BACK
// DepthStencil State   : LESS
// Blend State          : Default

// Parameter
 
// ===============


VS_MOTIONBLUR_OUT VS_Motion_Blur(VS_MOTIONBLUR_IN _in)
{
    
    VS_MOTIONBLUR_OUT output = (VS_MOTIONBLUR_OUT) 0.f;
    
    output.vUV = _in.vUV;
    output.vNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV));
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
   
    float4 vNewPos = output.vPosition;
    float4 vPrevPos = mul(float4(_in.vPos, 1.f), g_matPrevWorld);
    vPrevPos = mul(vPrevPos, g_matPrevView);
    vPrevPos = mul(vPrevPos, g_matPrevProj);
    
    //float4 vPos = mul(float4(_in.vPos, 1.f), g_matWorld);
    //float4 vPrev = mul(float4(_in.vPos, 1.f), g_matPrevWorld);
    
    //float3 vDir = vPos.xyz - vPrev.xyz;
    
    float3 vDir = output.vPosition.xyz - vPrevPos.xyz;
    float3 vAbsDir = abs(vDir);
    if (vAbsDir.x <= 0.05f && vAbsDir.y <= 0.05f)
    {
        output.vVelocity = float4(0.f, 0.f, 0.f, 0.f);
        return output;
    }
    
    //0~1���� ���缭
    float2 vVelocity = normalize(vNewPos.xy / vNewPos.w - vPrevPos.xy / vPrevPos.w);
    //UV��ǥ��� ��ȯ 
    vDir.xy = vVelocity.xy * 0.5f;
    vDir.xy *= -1; //(1.f - vVelocity.y / 2.f) - 0.5f;
    output.vVelocity.xy = vDir.xy;
  
    
   //�ӵ� �������� ������ġ ���
   // �� �븻�� �� ������ ��ġ�ϸ� ���� ��ġ 
   //�ƴϸ� ������ġ�� ��ġ
    float a = dot(normalize(output.vNormal), normalize(vDir));
   //if(a < 0.f)
   //{
   //     //�ٸ�����
   //     output.vPosition = vPrevPos;
   // }
   //else
   //{
   //    //��������
   //    output.vPosition = vNewPos;
   //}
    
    output.vVelocity.z = output.vPosition.z;
    output.vVelocity.w = output.vPosition.w;
   //����
   
    
    return output;
}


PS_MOTIONLUR_OUT PS_Motion_Blur(VS_MOTIONBLUR_OUT _in)
{
    PS_MOTIONLUR_OUT output = (PS_MOTIONLUR_OUT) 0.f;
    
    output.vVelocity.xy = _in.vVelocity.xy;
    
    output.vVelocity.w = _in.vVelocity.z / _in.vVelocity.w;
    //����

    if (abs(output.vVelocity.x) > 0.f || abs(output.vVelocity.y) > 0.f)
    {
        output.vData.x = 0.f;
    }
    else
    {
        output.vData.x = 1.f;
    }
    
    return output;
}





#endif