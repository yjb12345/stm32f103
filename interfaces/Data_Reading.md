# Data Reading 방법
```mermaid
---
title: 장비
---

%%{init: {"flowchart": {"htmlLabels": false}} }%%
flowchart LR
    CPU1[CPU]
    CPU2[Servere program]
    subgraph 장비
    온/습도칩--데이터 라인-->CPU1
    온/습도칩--인터럽트 라인-->CPU1
    end
    장비 --> Internet
    Internet<---->서버PC
    subgraph 서버PC
    CPU2
    end
    
```
---

## CPU에서 데이터 읽기
    1.1 pollong 방식
        (설정된 시간 단위로 CPU가 데이터 읽음)
    1.2 Interrup 방식
        (인터럽즈 발생시 데이터 읽음)
    1.3 DMA 방식 [Direct Memory Access]
        (자동으로 데이터가 CPU 있는 메모리로 들어옴)
---
## 서버 PC
    1. 데이터 IN
    2. Callback function

Call back fucntion

