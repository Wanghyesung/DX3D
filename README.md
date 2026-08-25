# DarkSouls DirectX3D

C++ / DirectX11로 밑바닥부터 직접 만든 자체 엔진 위에서 제작한 **3인칭 3D 소울라이크 액션 RPG**입니다.
렌더링·물리·길찾기·애니메이션까지 엔진 코드를 전부 1인으로 개발했습니다.

> 데모 영상: https://1drv.ms/v/c/d9470e8558d46727/IQBShbsPOFYYSKuibxMSLmObAWTLO_FL8DPUOcpxPnj-1wg?e=GEIpsc
<img width="631" height="413" alt="image" src="https://github.com/user-attachments/assets/dd003d20-5969-47ef-9ecf-6f6416335222" />


## 게임 소개

| 항목 | 내용 |
|---|---|
| 장르 | 3인칭 3D 소울라이크 액션 RPG |
| 개발 인원 | 1인 개발 (엔진 · 게임 로직 전담) |
| 개발 환경 | Visual Studio 2022, C++, DirectX 11 |
| 라이브러리 | PhysX 5.0, Recast & Detour, Autodesk FBX SDK, FMOD, DirectXTex |

플레이어 캐릭터 **Aritorias**가 보스 스테이지를 가로질러 일반 몬스터를 뚫고 보스 **소머리 데몬**을 격파하는 것이 목표인 짧은 전투 데모입니다.

## 조작법

| 입력 | 동작 |
|---|---|
| `W` `A` `S` `D` | 이동 |
| 마우스 이동 | 캐릭터 좌우 회전 |
| 마우스 좌클릭 | 공격 (콤보, 점프 중에도 가능) |
| `Space` (탭) | 구르기 |
| `Space` (홀드) | 달리기 |
| `Shift` (탭) | 점프 |
| `ESC` | 무기 교체 |

플레이어는 Idle → Walk/Run → Roll/Jump → Attack/JumpAttack → Hit 로 이어지는 상태 머신(FSM)으로 동작하며,
공격마다 데미지 · 넉백 · 최대 타격 수를 담은 정보로 콤보를 구성합니다.

## 보스 & 몬스터

- **소머리 데몬** (보스) — 다크소울1의 Taurus Demon을 모티브로 제작. Idle / Move / Wait / Jump / JumpAttack / Hit 전용 상태를 가진 별도 FSM으로 동작하며, 화면 상단에 전용 보스 체력 UI가 표시됩니다.
- **발더기사** (일반 몬스터) — Idle / Move / Attack / Hit / Dead의 기본 몬스터 FSM으로 동작하며, 머리 위 월드 스페이스 체력바를 달고 다닙니다.

## 핵심 기술

자체 엔진(`Project/Engine`)에 직접 구현한 기능들입니다.

- **PhysX 충돌 처리** — 브루트포스 전수비교(OBB) 방식을 PhysX 이벤트 기반 충돌로 전환해, 콜라이더 70개 환경에서 FPS 28 → 39로 개선
- **Recast/Detour 길찾기** — 정적 지형을 NavMesh로 빌드하고, Raycast로 배치한 동적 장애물을 몬스터가 실시간으로 우회
- **Frustum Culling** — 카메라 절두체 밖 오브젝트의 렌더 호출 자체를 생략해 FPS 28 → 36
- **Cascade Shadow Map** — 근경/중경/원경 3단계 해상도로 분리한 실시간 그림자
- **3D 스키닝 애니메이션** — FBX SDK로 로드한 본 애니메이션 재생 · 블렌딩
- **월드 스페이스 UI 빌보드** — 지오메트리 쉐이더로 카메라 각도와 무관하게 항상 정면을 향하는 몬스터 체력바
- **파티클 시스템**, **MRT 기반 렌더링**, **데칼 / 모션 블러 / 스카이박스 / 지형(LandScape)**, **FMOD 사운드**

## 프로젝트 구조

```
Project/
├─ Engine/    엔진 코어 — 렌더링, 물리, 길찾기, 애니메이션, UI, 사운드
├─ Script/    게임 로직 — 플레이어/몬스터 스크립트, FSM 상태
└─ Client/    실행 진입점 및 레벨 구성 (TestLevel.cpp)
External/     서드파티 SDK (PhysX, FBX SDK, FMOD 등) 및 엔진 공개 헤더
```

