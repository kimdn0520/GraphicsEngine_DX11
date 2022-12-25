# GraphicsEngine_DX11

## [그래픽스 엔진 개인 제작]

- **게임엔진**
    - Component Based GameEngine
    - Camera, Transform, MeshRenderer, SceneManager, Resources..
- **Parser**
    - ASEParser
    - FBXParser
- **그래픽스**
    - Pass 구조 개선
    - Shadow, Deferred Rendering, PBR
        
- **FBXLoader**
    - FBX sdk를 활용한 FBXLoad를 하자
    - Boost Library를 활용한 BinarySerialization을 한다. -> bin 폴더에 저장
    - Resources 에서 bin폴더에 저장된 자체포맷 파일을 DeSerialization을 해서 제대로 Load 가 되는지 테스트한다. -> 성공
    - FBXLoader를 분리하고 하나의 애플리케이션으로 만든다 -> 실행시 FBXLoad -> 자체포맷 -> bin 폴더에 저장
    - boost를 Library를 사용해서 Serialization, Deserialization을 하였고 내 엔진에서 불러오기가 되었다.
    - 프로젝트 CarlTale 팀 엔진에 이식을 하였고 모델 불러오기 성공
 
