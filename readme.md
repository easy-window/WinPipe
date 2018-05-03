# Windows Pipe 
Windows 管道 Pipe 封装类库，可用于进程间通信。

# Sample

```cpp
// Create Pipe
#define EXAMP_PIPE   _T("\\\\.\\pipe\\ReadPipe")
#define MAX_SIZE    10     // 管道最大通信 buffer size

CPipe pipe;
pipe.Create(EXAMP_PIPE, MAX_SIZE);
pipe.SetRecvCallback(&RecvCallback, this);

// Buffer 为收到的字节，nSize 为字节大小，lpParam 为自定义的变量指针
static void CALLBACK RecvCallback(void* Buffer, int nSize, LPVOID lpParam)
{
    // do something
    int* pData = (int*) Buffer;
    cout << *pData << endl;
}

// close pipe
pipe.Close();

```
```cpp
// Connect Pipe
#define EXAMP_PIPE   _T("\\\\.\\pipe\\ReadPipe")

int nData = 1024;

CPipe pipe;
pipe.Connect(EXAMP_PIPE);
pipe.Send(nData, sizeof(int));

// close pipe
pipe.Close();
```