#ifndef _FUNC
#define _FUNC

#include "value.fx"

void CalcLight2D(float3 _vWorldPos, inout tLightColor _Light)
{
    for (uint i = 0; i < g_Light2DCount; ++i)
    {
        if (g_Light2DBuffer[i].LightType == 0)
        {
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb;
            _Light.vAmbient.rgb += g_Light2DBuffer[i].Color.vAmbient.rgb;
        }
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);

            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
        
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fPow;
        }
        else if (g_Light2DBuffer[i].LightType == 2)
        {
            
        }
    }
}

void CalcLight2D(float3 _vWorldPos, float3 _vWorldDir, inout tLightColor _Light)
{
    for (uint i = 0; i < g_Light2DCount; ++i)
    {
        if (g_Light2DBuffer[i].LightType == 0)
        {
            float fDiffusePow = saturate(dot(-g_Light2DBuffer[i].vWorldDir.xyz, _vWorldDir));
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow;
            _Light.vAmbient.rgb += g_Light2DBuffer[i].Color.vAmbient.rgb;
        }
        else if (g_Light2DBuffer[i].LightType == 1)
        {
            float3 vLightWorldPos = float3(g_Light2DBuffer[i].vWorldPos.xy, 0.f);
            float3 vWorldPos = float3(_vWorldPos.xy, 0.f);

            // 광원 중심에서 물체를 향하는 방향
            float3 vLight = normalize(vWorldPos - vLightWorldPos);
            float fDiffusePow = saturate(dot(-vLight, _vWorldDir));
            
            float fDistance = abs(distance(vWorldPos, vLightWorldPos));
            float fDistPow = saturate(1.f - (fDistance / g_Light2DBuffer[i].Radius));
        
            _Light.vDiffuse.rgb += g_Light2DBuffer[i].Color.vDiffuse.rgb * fDiffusePow * fDistPow;
        }
        else if (g_Light2DBuffer[i].LightType == 2)
        {
            
        }
    }
}

void CalcLight3D(float3 _vViewPos, float3 _vViewNormal, int _LightIdx, inout tLightColor _Light, inout float _SpecPow)
{
    //_vViewPos는 호출된 픽셀의 위치
    
    tLightInfo LightInfo = g_Light3DBuffer[_LightIdx];
    
    float fLightPow = 0.f;
    float fSpecPow = 0.f;
    float3 vViewLightDir = (float3) 0.f;
        
    // DirLight 인 경우
    if (0 == LightInfo.LightType)
    {
        // ViewSpace 에서의 광원의 방향
        vViewLightDir = normalize(mul(float4(normalize(LightInfo.vWorldDir.xyz), 0.f), g_matView)).xyz;
        
        // ViewSpace 에서의 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)    
        fLightPow = saturate(dot(_vViewNormal, -vViewLightDir));
        
        // 반사광
        float3 vViewReflect = normalize(vViewLightDir + 2.f * (dot(-vViewLightDir, _vViewNormal)) * _vViewNormal);
        float3 vEye = normalize(_vViewPos);
        
        // 반사광의 세기 구하기
        fSpecPow = saturate(dot(vViewReflect, -vEye));
        fSpecPow = pow(fSpecPow, 20);
    }
    
    // Point Light 인 경우
    else if (1 == LightInfo.LightType)
    {
        float fDistPow = 1.f;
        
        // ViewSpace 에서의 광원의 위치
        float3 vLightViewPos = mul(float4(LightInfo.vWorldPos.xyz, 1.f), g_matView).xyz;
        
        // 광원으로부터 오는 빛의 방향 구하기
        vViewLightDir = normalize(_vViewPos - vLightViewPos);
        
        
        // 포인트 라이트로부터 거리체크
        float fDist = distance(_vViewPos, vLightViewPos);
        fDistPow = saturate(cos((fDist / LightInfo.Radius) * (PI / 2.f)));
                               
        // ViewSpace 에서의 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)    
        fLightPow = saturate(dot(_vViewNormal, -vViewLightDir)) * fDistPow;
        
        // 반사광
        float3 vViewReflect = normalize(vViewLightDir + 2.f * (dot(-vViewLightDir, _vViewNormal)) * _vViewNormal);
        float3 vEye = normalize(_vViewPos);
        
        // 반사광의 세기 구하기
        fSpecPow = saturate(dot(vViewReflect, -vEye));
        fSpecPow = pow(fSpecPow, 20) * fDistPow;
    }
    
    // Spot Light 인 경우
    else
    {
       //내 빛의 위치
        float3 vLightViewPos = mul(float4(LightInfo.vWorldPos.xyz, 1.f), g_matView).xyz;
        
        //빛에서 내 픽셀로 가는 방향 벡터
        float3 vDir = normalize(_vViewPos - vLightViewPos);
        
        //내 빛의 방향
        vViewLightDir = normalize(mul(float4(normalize(LightInfo.vWorldDir.xyz), 0.f), g_matView)).xyz;
        
        //내 각도
        float fDotCos = dot(vViewLightDir, vDir);
        
        //최대 각도 범위
        float fCos = cos(LightInfo.Angle);
        
        if (fDotCos < fCos)
        {
            fLightPow = 0.f;
        }
        else
        {
            float fDist = distance(_vViewPos, vLightViewPos);
            float fDistPow = saturate(1.f - (fDist / LightInfo.Radius));
        
        //반사광
            float3 vViewReflect = normalize(vViewLightDir + 2.f * (dot(-vViewLightDir, _vViewNormal)) * _vViewNormal);
            float3 vEye = normalize(_vViewPos);
        
         //viewspac에서 노말벡터와 광원의 방향을 내적 (램버트 코사인 법칙)
            fLightPow = saturate(dot(_vViewNormal, -vViewLightDir)) * fDistPow;
        
         //반사광의 세기 구하기
            fSpecPow = saturate(dot(vViewReflect, -vEye)) * fDistPow;
            fSpecPow = pow(fSpecPow, 20);
        }
    }
      
    // 결과 전달하기
    _Light.vDiffuse += LightInfo.Color.vDiffuse * fLightPow;
    _Light.vAmbient += LightInfo.Color.vAmbient;
    _SpecPow += fSpecPow;
}

// ======
// Random
// ======
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

void GaussianSample(in Texture2D _NoiseTex, float2 _vResolution, float _NomalizedThreadID, out float3 _vOut)
{
    float2 vUV = float2(_NomalizedThreadID, 0.5f);
    
    vUV.x += g_AccTime * 0.5f;
    
    // sin 그래프로 텍스쳐의 샘플링 위치 UV 를 계산
    vUV.y -= (sin((_NomalizedThreadID - (g_AccTime /*그래프 우측 이동 속도*/)) * 2.f * 3.1415926535f * 10.f /*반복주기*/) / 2.f);
    
    if (1.f < vUV.x)
        vUV.x = frac(vUV.x);
    else if (vUV.x < 0.f)
        vUV.x = 1.f + frac(vUV.x);
    
    if (1.f < vUV.y)
        vUV.y = frac(vUV.y);
    else if (vUV.y < 0.f)
        vUV.y = 1.f + frac(vUV.y);
        
    int2 pixel = vUV * _vResolution;
    int2 offset = int2(-2, -2);
    float3 vOut = (float3) 0.f;
    
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            vOut += _NoiseTex[pixel + offset + int2(j, i)].xyz * GaussianFilter[i][j];
        }
    }
    
    _vOut = vOut;
}


matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    //열(row) = 0
    //같은 물체에서 몇번째로 그릴지 _iRowIdx 
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        //
        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}

void AnimationSkinning(inout float3 _vPos, inout float4 _vWeight, inout float4 _vIndices, int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
}

int IntersectsLay(float3 _vertices[3], float3 _vStart, float3 _vDir, out float3 _vCrossPoint, out float _fResult)
{
    //지형 로컬영역에서의 _vDir
    
    //직선의 방정식 R(t) = v1 + t * vDir
    //R(t) 직선상의 모든 점
    //v1 직선상의 임의 한 점
    //t : v1로 부터 떨어진 거리
    //vDir 직선의 방향(벡터)
    
    //내적 : (d)
    // pN d p1 +d(점과 평면의 단거리) = 0
    //평면의 방정식 : pN d (p0 - p1) = 0
    //pn 평면에 수직인방향벡터
    //p0 평면상의 임의 한 점
    //p1 평면상의 임의의 한 점
    //(p0 - p1) 평면위의 선분(벡터)
    
    //위의 두 방정식이 있고 직선의 교차점이 평면상의 임의의 한 점이 되므로
    //p1 = R(t)가 된다
 
    
    float3 edge[2] = { (float3) 0.f, (float3) 0.f };
    edge[0] = _vertices[1].xyz - _vertices[0].xyz;
    edge[1] = _vertices[2].xyz - _vertices[0].xyz;
        
     //t = (pN d p0 - pn d v1) / pN d vDir
    float3 normal = normalize(cross(edge[0], edge[1]));
    
    //광선의 방향과 표면의 방향을 내적하여 얼마나 일치하는지 확인 b =0 이면 광선이 평면에 수직한 방향
    //각도를 구한다
    float b = dot(normal, _vDir);

    //삼각형 점에서 점의 시작점까지 거리
    float3 v1 = _vStart - _vertices[0].xyz;
    
   
    //이 값은 광선의 시작점에서 삼각형 평면까지의 수직 거리를 나타냅니다. (a = -dot(normal, v1))
    //내적은 두 벡터의 방향이 얼마나 비슷한지를 나타내는데, 음수 값은 두 벡터의 방향이 반대임을 의미합니다.
    //평면의 법선벡터와 삼각형 점에서 시작점까지의 벡터를 내적하여 스칼라값을 구한다. y값
    
    float a = -dot(normal, v1);
    
    float t = a / b;

    //거리
    _fResult = t;

    //교차 점
    float3 p = _vStart + t * _vDir;
    _vCrossPoint = p;
    
    float uu, uv, vv, wu, wv, inverseD;
    uu = dot(edge[0], edge[0]);
    uv = dot(edge[0], edge[1]);
    vv = dot(edge[1], edge[1]);

    
    float3 w = p - _vertices[0].xyz;
    wu = dot(w, edge[0]);
    wv = dot(w, edge[1]);
    inverseD = uv * uv - uu * vv;
    inverseD = 1.0f / inverseD;

    float u = (uv * wv - vv * wu) * inverseD;
    if (u < 0.0f || u > 1.0f)
    {
        return 0;
    }

    float v = (uv * wu - uu * wv) * inverseD;
    if (v < 0.0f || (u + v) > 1.0f)
    {
        return 0;
    }

    return 1;
}


float GetTessFactor(float _Length, int _iMinLevel, int _iMaxLevel, float _MinDistance, float _MaxDistance)
{
    if (_MaxDistance < _Length)
    {
        return 0.f;
    }
    else if (_Length < _MinDistance)
    {
        return _iMaxLevel;
    }
    else
    {
        float fLevel = _iMaxLevel - (_iMaxLevel - _iMinLevel) * ((_Length - _MinDistance) / (_MaxDistance - _MinDistance));

        return fLevel;
    }
}

int GetShadowIdx(float _fDistance)
{
    float fDistacne = abs(_fDistance);
    if (fDistacne < 2000.f)  
        return 0; // 근경 
    else if (fDistacne < 4000.f)
        return 1; // 중경 
    else
        return 2; // 원경 
}

static const float2 HardOffset[8] =
{
    float2(0.0, 1.0), 
    float2(0.707, 0.707), 
    float2(1.0, 0.0), 
    float2(0.707, -0.707), 
    float2(0.0, -1.0), 
    float2(-0.707, -0.707), 
    float2(-1.0, 0.0), 
    float2(-0.707, 0.707) 
};

// 4각 샘플 오프셋
static const float2 SimpleOffset[4] =
{
    float2(-0.5, -0.5), float2(0.5, -0.5),
    float2(-0.5, 0.5), float2(0.5, 0.5)
};

//#define ShadowTargetTex g_tex_4
//float CalcShadow(float2 _fUV, float _fDistance)
//{
//    int iCount = GetShadowSampleCount(_fDistance);
    
//    float fShadowPow = 0.f;
//    if(iCount == 8)
//    {
//        for (int i = 0; i < 8; ++i)
//        {
//            float2 fsampleOffset = HardOffset[i] * 0.005; // 오프셋 크기 조정
//            fShadowPow += ShadowTargetTex.Sample(g_sam_0, _fUV + fsampleOffset).r;
//        }
//        fShadowPow /= 8.f;
//    }
//    else if(iCount ==4)
//    {
//        for (int i = 0; i < 4; ++i)
//        {
//            float2 fsampleOffset = SimpleOffset[i] * 0.001; // 오프셋 크기 조정
//            fShadowPow += ShadowTargetTex.Sample(g_sam_0, _fUV + fsampleOffset).r;
//        }
//        fShadowPow /= 4.f;
//    }
//    else
//    {
//        fShadowPow = ShadowTargetTex.Sample(g_sam_0, _fUV + _fUV).r;
//    }
    
//    return fShadowPow;
//}

#define NearShadowMapTargetTex g_tex_2
#define MidShadowMapTargetTex g_tex_3
#define FarShadowMapTargetTex g_tex_4

float CalcShadow(float2 _fUV, float _fDistance)
{
    int iIdx = GetShadowIdx(_fDistance);
    float fShadowPow = 0.f;
    
    if (iIdx == 0)
        fShadowPow = NearShadowMapTargetTex.Sample(g_sam_1, _fUV).r + 0.0005;
    else if (iIdx == 1)
        fShadowPow = MidShadowMapTargetTex.Sample(g_sam_1, _fUV).r + 0.005f;
    else
        fShadowPow = FarShadowMapTargetTex.Sample(g_sam_1, _fUV).r + 0.01f;
    
    return fShadowPow;
}


#endif
