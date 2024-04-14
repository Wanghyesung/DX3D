#ifndef _VALUE
#define _VALUE

#include "struct.fx"

cbuffer TRANSFORM : register(b0)
{
    
    //g_matWVP : 해당 vertexshader(Vs_Std2D)를 사용하는 물체의 월드상의 크기, 회전
    //, 이동 정보를 정장하고 있는 행렬
    //b0 : gpu가 사용할 수 있게 상수레지스터에 바인딩을 해둠
    row_major matrix g_matWorld;
    row_major matrix g_matWorldInv;
    row_major Matrix g_matPrevWorld;
    
    row_major matrix g_matView;
    row_major matrix g_matViewInv;
    row_major Matrix g_matPrevView;
    
    row_major matrix g_matProj;
    row_major matrix g_matProjInv;
    row_major Matrix g_matPrevProj;
    
    //Inv 역행렬
    
    row_major matrix g_matWV;
    row_major matrix g_matWVP;
};

cbuffer MATERIAL : register(b1)
{
    float4 g_bDiff;
    float4 g_vSpec;
    float4 g_vAmb;
    float4 g_vEmv;
    
    int g_int_0;
    int g_int_1;
    int g_int_2;
    int g_int_3;
    
    float g_float_0;
    float g_float_1;
    float g_float_2;
    float g_float_3;
    
    float2 g_vec2_0;
    float2 g_vec2_1;
    float2 g_vec2_2;
    float2 g_vec2_3;

    float4 g_vec4_0;
    float4 g_vec4_1;
    float4 g_vec4_2;
    float4 g_vec4_3;
    
    row_major matrix g_mat_0;
    row_major matrix g_mat_1;
    row_major matrix g_mat_2;
    row_major matrix g_mat_3;
    
    int g_btex_0;
    int g_btex_1;
    int g_btex_2;
    int g_btex_3;
    int g_btex_4;
    int g_btex_5;
    int g_btex_6;
    int g_btex_7;
    int g_btexcube_0;
    int g_btexcube_1;
    int g_btexarr_0;
    int g_btexarr_1;
    
    //3d Animation 정보
    int g_iAnim; //
    int g_iBoneCount; //내 뼈 행렬 개수
    int2 padding;
};

cbuffer GLOBAL : register(b2)
{
    float4 g_CameraRot;
    float2 g_Resolution;
    float g_DT;
    float g_AccTime;
    uint g_Light2DCount;
    uint g_Light3DCount;
    int2 g_globalpadding;
}

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);
Texture2D g_tex_5 : register(t5);
Texture2D g_tex_6 : register(t6);
Texture2D g_tex_7 : register(t7);

TextureCube g_cube_0 : register(t8);
TextureCube g_cube_1 : register(t9);

Texture2DArray g_arrtex_0 : register(t10);
Texture2DArray g_arrtex_1 : register(t11);

StructuredBuffer<tLightInfo> g_Light2DBuffer : register(t12);
StructuredBuffer<tLightInfo> g_Light3DBuffer : register(t13);

//Animation3D Bone Matrix Buffer
//이번 프레임에 동작하는 뼈 행렬
StructuredBuffer<Matrix> g_arrBoneMat : register(t30);

SamplerState g_sam_0 : register(s0); //포인트
SamplerState g_sam_1 : register(s1);

#define PI 3.141592f

#endif