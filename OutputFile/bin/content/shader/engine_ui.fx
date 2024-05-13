#ifndef _ENGINE_UI
#define _ENGINE_UI

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vLocalPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

// ============================
// engine_ui
// RasterizerState      : None
// BlendState           : Mask
// DepthStencilState    : Less
//
// Parameter
// g_int_0              : 3DobjectUI
// g_vec2_0             : AnimAtlas Size
//
// g_tex_0              : Output Texture
// ============================

#define IS3D        g_int_0
#define UI_SIZE     g_vec2_0
#define CAM_POS     g_vec4_0
VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    if(IS3D)
    {
        //원래쓰던 방법으로 다시 해보기
        //float3 viewportVector = normalize(mul(float4(CAM_POS.xyz, 1.f), g_matView)).xyz;

        float3 vFoward = float3(0.f, 0.f, 1.f);
        float3 vWorldPos = mul(float4(_in.vLocalPos.xyz, 1.f), g_matWorld).xyz;
        float3 vDir = normalize(CAM_POS.xyz - vWorldPos).xyz;
        
      
        float fTheta = acos(dot(vDir, vFoward));
        
       //if (vDir.y < vFoward.y)
       //{
            fTheta = (2.f * 3.1415926535f) - fTheta;
       // }
        
        
        float3x3 matRotZ =
        {
            cos(fTheta),  0,           -sin(fTheta),
            0,            1.f,         0,
            sin(fTheta), 0,           cos(fTheta)
        };
        
        float3 vRotate = mul(_in.vLocalPos.xzy, matRotZ);
        
        float4 vView = mul(float4(vWorldPos, 1.f), g_matView);
        output.vPosition = mul(float4(vRotate + vView.xyz, 1.f), g_matProj);
    }
    else
    {
        output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    }
      
    output.vUV = _in.vUV;
    
    return output;
}

// 레스터라이저 스테이트
float4 PS_Std2D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
        
    if (g_btex_0)
    {
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }

    if (0.f == vOutColor.a)
        discard; // 픽셀 쉐이더 중단
    
    
    float fRatio = UI_SIZE.x / 100.f;
    if (_in.vUV.x > fRatio)
        discard;
    
    return vOutColor;
}


#endif