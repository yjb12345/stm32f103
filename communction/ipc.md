# 2024-06-02
1. ## 작업 내용

IPC : Inter=process Commcuication

```mermaid
---
title: IPC
---

%%{init: {"flowchart": {"htmlLabels": false}} }%%
flowchart LR
    PC1["pc.kr"]
    PC2["pc.us"]    
    PC1 <--"HTTPS/HTTP\nTCP/UDP"--> PC2
```
*HTTP :  Hypertext Transfer Protocol
*HTTPS:
*UDP
*TCP


---
## Communication
```mermaid
---
title: Communication with TCP
---

%%{init: {"flowchart": {"htmlLabels": false}} }%%
flowchart LR
    
    subgraph client
     Drawer
     A{{Commwith Main Server}}
     B{{with Chatting Server}}
    end
    
    A<--"TCP"-->GMC["Game Main Server"]
    B<--"TCP"-->CHS["Chatting Server"]
    
```


---
## IPC(Inter-Process Communicaion)
```mermaid
---
title: Communication with Signal or Handle
---

%%{init: {"flowchart": {"htmlLabels": false}} }%%
flowchart LR
    
    subgraph client
     D{{Drawer}}
     A{{Commwith Main Server}}
     B{{with Chatting Server}}
    D<--"Message"-->A
    D<--"Message"-->B
    end    
    A<--"TCP"-->GMC["Game Main Server"]
    B<--"TCP"-->CHS["Chatting Server"]
    

```
###IPC 정리
- Linux : Signal, MsgQ
- Windows : Message, Singal
```
PostMessage
SendMessag  e
```


