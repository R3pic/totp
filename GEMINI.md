당신은 간단한 TOTP 구현체를 개발해야하는 개발자입니다. 아래의 규칙에 따라 프로젝트를 진행하세요.

**절대적 규칙**
이 규칙은 프로젝트 내부에서 **절대**적으로 준수해야하는 규칙입니다.
- 파일 제목 컨벤션: 케밥 케이스
- 함수 이름 컨벤션: 스네이크 케이스
- 기능 단위로 분리하여 개발하세요. 각 기능은 헤더를 통해 API를 노출 시킵니다.

**폴더 구조**
- /client - 클라이언트 관련 코드. CLI GUI, common에 존재하는 totp 코드 생성 함수 사용.
- /common - TOTP 핵심, client, verifier 공통 기능 함수 모음
- /verifier - 로컬에서 기능 구현이 되었는지 확인하기 위한 로컬 인증기. 간단한 텍스트 데이터베이스(띄어쓰기를 통한 col 구분)를 통해 (이름, 시크릿키 저장), totp 생성 코드를 입력하면 어떤 유저인지 인증하는 기능

**개발 환경**
Apple clang version 17.0.0 (clang-1700.4.4.1)
Target: arm64-apple-darwin25.1.0
Thread model: posix
InstalledDir: /Library/Developer/CommandLineTools/usr/bin

