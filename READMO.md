# Win32 TCP Socket  관련 소스코드

## API 정의

### 1. 소켓 생성 API
```
SOCKET WSAAPI socket(
    [in]    int af,
    [in]    int type,
    [in]    int protocol
);
```
### 2. 소켓 포트 바인드 API
```
u_short htons(
    [in]    u_short hosthort
);

u_long htonl(
    [in]    u_long  hostlong
);

int bind(
    [in]    SOCKET          s,
            const sockaddr  *addr,
    [in]    int             namelen
);

```
### 3. 소켓 접속 대기 API
```
int WSAAPI  listen(
    [in]    SOCKET  s,
    [in]    int     backlog
);
```
### 4. 소켓 클라이언트 접속 API
```
SOCKET WSAAPI accept(
    [in]         SOCKET      s,
    [out]       sockaddr    *addr,
    [in, out]   int         *addrlen
);
```
### 5. 소켓 수신 API
```
int WSAAPI recv(
    [in]    SOCKET  s,
    [out]   char    *buf,
    [in]    int     len,
    [in]    int     flags
);
```
### 6. 소켓 송신 API
```
int WSAAPI send(
    [in]    SOCKET      s,
    [out]   const char  *buf,
    [in]    int         len,
    [in]    int         flags
);
```
### 7. 소켓 송/수신 차단 API
```
int shutdown(
    [in]    SOCKET  s,
    [in]    int     how
);
```
### 8. 소켓 닫기 API 
```
int closesocket(
    [in]    SOCKET  s
);
```
