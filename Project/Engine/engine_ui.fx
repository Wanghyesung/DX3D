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

#define IS_GAGE     g_int_0
#define UI_SIZE     g_vec2_0

VS_OUT VS_Std2D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vLocalPos, 1.f), g_matWVP);
    output.vUV = _in.vUV;
    
    return output;
}

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
        discard; 
    
    
    if (IS_GAGE)
    {
        float fRatio = UI_SIZE.x / 100.f;
        if (_in.vUV.x > fRatio)
            discard;
    }

    return vOutColor;
}


// ============================================================
// World-space Billboard UI (monster HP/Frame etc. - always faces camera)
// RasterizerState      : CULL_NONE
// BlendState           : ALPHA_BLEND
// DepthStencilState    : LESS
//
// mesh                 : PointMesh (single vertex, marks world position)
// topology             : POINTLIST
//
// Parameter
// g_int_0  (IS_GAGE)   : gage cutoff on/off
// g_vec2_0 (UI_SIZE)   : gage fill ratio (0~100)
// g_vec2_1 (WORLD_SIZE): billboard quad size in world units
//
// g_tex_0              : Output Texture
// ============================================================
#define WORLD_SIZE  g_vec2_1

struct VS_WorldUI_OUT
{
    float3 vPos : POSITION;
};

// Pass the PointMesh's local position (0,0,0) through untouched.
// The actual world/view transform happens in the geometry shader.
VS_WorldUI_OUT VS_WorldUIRender(VS_IN _in)
{
    VS_WorldUI_OUT output = (VS_WorldUI_OUT) 0.f;

    output.vPos = _in.vLocalPos;

    return output;
}

struct GS_WorldUI_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
};

[maxvertexcount(6)]
void GS_WorldUIRender(point VS_WorldUI_OUT _in[1], inout TriangleStream<GS_WorldUI_OUT> _outstream)
{
    GS_WorldUI_OUT output[4] = 
        { (GS_WorldUI_OUT) 0.f, (GS_WorldUI_OUT) 0.f, (GS_WorldUI_OUT) 0.f, (GS_WorldUI_OUT) 0.f };

    float4 worldPos4 = mul(float4(_in[0].vPos, 1.0f), g_matWorld);
    float4 viewPos4 = mul(worldPos4, g_matView);
    float3 vViewPos = viewPos4.xyz;
    // 0 -- 1
    // |    |
    // 3 -- 2
    float3 NewPos[4] =
    {
        float3(-WORLD_SIZE.x / 2.f, +WORLD_SIZE.y / 2.f, 0.f),
        float3(+WORLD_SIZE.x / 2.f, +WORLD_SIZE.y / 2.f, 0.f),
        float3(+WORLD_SIZE.x / 2.f, -WORLD_SIZE.y / 2.f, 0.f),
        float3(-WORLD_SIZE.x / 2.f, -WORLD_SIZE.y / 2.f, 0.f)
    };

    output[0].vPosition = mul(float4(NewPos[0] + vViewPos, 1.f), g_matProj);
    output[0].vUV = float2(0.f, 0.f);

    output[1].vPosition = mul(float4(NewPos[1] + vViewPos, 1.f), g_matProj);
    output[1].vUV = float2(1.f, 0.f);

    output[2].vPosition = mul(float4(NewPos[2] + vViewPos, 1.f), g_matProj);
    output[2].vUV = float2(1.f, 1.f);

    output[3].vPosition = mul(float4(NewPos[3] + vViewPos, 1.f), g_matProj);
    output[3].vUV = float2(0.f, 1.f);

    _outstream.Append(output[0]); _outstream.Append(output[1]); _outstream.Append(output[2]);
    _outstream.RestartStrip();

    _outstream.Append(output[0]); _outstream.Append(output[2]); _outstream.Append(output[3]);
    _outstream.RestartStrip();
}

float4 PS_WorldUIRender(GS_WorldUI_OUT _in) : SV_Target
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
        discard;

    if (IS_GAGE)
    {
        float fRatio = UI_SIZE.x / 100.f;
        if (_in.vUV.x > fRatio)
            discard;
    }

    return vOutColor;
}


#endif