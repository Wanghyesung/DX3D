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
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
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