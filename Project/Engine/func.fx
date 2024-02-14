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

            // ���� �߽ɿ��� ��ü�� ���ϴ� ����
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
    tLightInfo LightInfo = g_Light3DBuffer[_LightIdx];
    
    float fLightPow = 0.f;
    float fSpecPow = 0.f;
    float3 vViewLightDir = (float3) 0.f;
        
    // DirLight �� ���
    if (0 == LightInfo.LightType)
    {
        // ViewSpace ������ ������ ����
        vViewLightDir = normalize(mul(float4(normalize(LightInfo.vWorldDir.xyz), 0.f), g_matView)).xyz;
        
        // ViewSpace ������ �븻���Ϳ� ������ ������ ���� (����Ʈ �ڻ��� ��Ģ)    
        fLightPow = saturate(dot(_vViewNormal, -vViewLightDir));
        
        // �ݻ籤
        float3 vViewReflect = normalize(vViewLightDir + 2.f * (dot(-vViewLightDir, _vViewNormal)) * _vViewNormal);
        float3 vEye = normalize(_vViewPos);
        
        // �ݻ籤�� ���� ���ϱ�
        fSpecPow = saturate(dot(vViewReflect, -vEye));
        fSpecPow = pow(fSpecPow, 20);
    }
    
    // Point Light �� ���
    else if (1 == LightInfo.LightType)
    {
        float fDistPow = 1.f;
        
        // ViewSpace ������ ������ ��ġ
        float3 vLightViewPos = mul(float4(LightInfo.vWorldPos.xyz, 1.f), g_matView).xyz;
        
        // �������κ��� ���� ���� ���� ���ϱ�
        vViewLightDir = normalize(_vViewPos - vLightViewPos);
        
        
        // ����Ʈ ����Ʈ�κ��� �Ÿ�üũ
        float fDist = distance(_vViewPos, vLightViewPos);
        fDistPow = saturate(cos((fDist / LightInfo.Radius) * (PI / 2.f)));
                               
        // ViewSpace ������ �븻���Ϳ� ������ ������ ���� (����Ʈ �ڻ��� ��Ģ)    
        fLightPow = saturate(dot(_vViewNormal, -vViewLightDir)) * fDistPow;
        
        // �ݻ籤
        float3 vViewReflect = normalize(vViewLightDir + 2.f * (dot(-vViewLightDir, _vViewNormal)) * _vViewNormal);
        float3 vEye = normalize(_vViewPos);
        
        // �ݻ籤�� ���� ���ϱ�
        fSpecPow = saturate(dot(vViewReflect, -vEye));
        fSpecPow = pow(fSpecPow, 20) * fDistPow;
    }
    
    // Spot Light �� ���
    else
    {
        // LightDir �� Angle ���� Ȱ���ؼ� SpotLight �����غ���
        
        // ����Ʈ ����Ʈ�� ����
    }
      
    // ��� �����ϱ�
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
    
    // sin �׷����� �ؽ����� ���ø� ��ġ UV �� ���
    vUV.y -= (sin((_NomalizedThreadID - (g_AccTime /*�׷��� ���� �̵� �ӵ�*/)) * 2.f * 3.1415926535f * 10.f /*�ݺ��ֱ�*/) / 2.f);
    
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
    //��(row) = 0
    //���� ��ü���� ���°�� �׸��� _iRowIdx 
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
    //���� ���ÿ��������� _vDir
    
    //������ ������ R(t) = v1 + t * vDir
    //R(t) �������� ��� ��
    //v1 �������� ���� �� ��
    //t : v1�� ���� ������ �Ÿ�
    //vDir ������ ����(����)
    
    //���� : (d)
    // pN d p1 +d(���� ����� �ܰŸ�) = 0
    //����� ������ : pN d (p0 - p1) = 0
    //pn ��鿡 �����ι��⺤��
    //p0 ������ ���� �� ��
    //p1 ������ ������ �� ��
    //(p0 - p1) ������� ����(����)
    
    //���� �� �������� �ְ� ������ �������� ������ ������ �� ���� �ǹǷ�
    //p1 = R(t)�� �ȴ�
 
    
    float3 edge[2] = { (float3) 0.f, (float3) 0.f };
    edge[0] = _vertices[1].xyz - _vertices[0].xyz;
    edge[1] = _vertices[2].xyz - _vertices[0].xyz;
        
     //t = (pN d p0 - pn d v1) / pN d vDir
    float3 normal = normalize(cross(edge[0], edge[1]));
    //�������� ����� ǥ���� ������ �����Ͽ� �󸶳� ��ġ�ϴ��� Ȯ�� b =0 �̸� ������ ��鿡 ������ ����
    //������ ���Ѵ�
    float b = dot(normal, _vDir);

    //������ ���������� �ﰢ�� �������� �Ÿ�
    float3 v1 = _vStart - _vertices[0].xyz;
    
    //���� a�� ������ ������ _vStart���� �ﰢ�� ��� ���� �� �������� ���Ϳ� �ﰢ���� ���� ���� normal�� ������ ���Դϴ�.
    //�� ���� ������ ���������� �ﰢ�� �������� ���� �Ÿ��� ��Ÿ���ϴ�. (a = -dot(normal, v1))
    //������ �� ������ ������ �󸶳� ��������� ��Ÿ���µ�, ���� ���� �� ������ ������ �ݴ����� �ǹ��մϴ�.
    //����, a�� ������ ���������� �ﰢ�� �������� ���� �Ÿ��� ��Ÿ���µ�,
    //������ ��Ÿ�����ν� ������ �ﰢ�� ������κ��� �Ʒ��ʿ� ��ġ�ϰ� �ִٴ� ���� �� �� �ֽ��ϴ�.
    float a = -dot(normal, v1);
    
    float t = a / b;

    //�Ÿ�
    _fResult = t;

    //���� ��
    float3 p = _vStart + t * _vDir;
    _vCrossPoint = p;
    
    //Triangle-Triangle Intersection
    //�ﰢ�� ������ �ﰢ�� ABC�� �� �������� ���� Barycentric ��ǥ (u, v, w)�� ������ ���� ���ǵ˴ϴ�.
    //P = u * A + v * B + w * C
    //���⼭ (u, v, w)�� Barycentric ��ǥ�̸�, u + v + w = 1�̾�� �մϴ�.
    //�ﰢ���� �� �� P�� �ﰢ�� ���ο� �ִٸ�, �� �� P�� �����ϴ� Barycentric ��ǥ (u, v, w)�� ��� 0���� 1 ������ ���� ������ �մϴ�. 
    //�ڵ忡�� u�� v�� �̷��� ������ �˻��ϰ�, (u + v)�� ���� 1�� ���� �ʵ��� Ȯ���ϰ� �ֽ��ϴ�.
    
    //https://wjdgh283.tistory.com/entry/%EC%82%BC%EA%B0%81%ED%98%95-%EC%95%88%EC%97%90-%EC%9E%88%EB%8A%94-%EC%A0%90%EC%9D%98-%EB%AC%B4%EA%B2%8C%EC%A4%91%EC%8B%AC-%EC%A2%8C%ED%91%9Cbarycentric-coordinate-%EA%B5%AC%ED%95%98%EA%B8%B0
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


#endif
