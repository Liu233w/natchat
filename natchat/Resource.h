//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ 生成的包含文件。
// 供 natchat.rc 使用
//

#define IDM_ABOUTBOX                    0x0010
#define IDD_ABOUTBOX                    100
#define IDS_ABOUTBOX                    101
#define IDD_NATCHAT_DIALOG              102
#define IDP_SOCKETS_INIT_FAILED         103
#define IDR_MAINFRAME                   128
#define IDD_SENDBOX                     130
#define IDD_REFRESH                     132
#define IDC_RECRIVERHOST                1012
#define IDC_IPLIST                      1013
#define IDC_RECEIVERTEXT                1014
#define IDC_RECEIVERTEXT2               1015
#define IDC_RECRIVERIP                  1016
#define IDC_FILETEXT                    1017
#define IDC_EDIT1                       1018
#define IDC_EDIT2                       1019
#define IDC_SENDFILE                    1023
#define IDC_SELECTEDFILE                1024
#define IDC_STATUS                      1025
#define IDC_CHOOSEFILE                  1026
#define IDC_SENDPRIVATE                 1027
#define IDC_CANCEL                      1028
#define IDC_PRIVATEEDIT                 1029
#define IDC_REFRESH                     1030
#define IDC_PRIVATETEXT                 1031
#define IDC_REFRESHING                  1032
#define IDC_SENDANI                     1033
#define IDC_EMOTIONANI                  1034
#define IDC_STATUSTEXT                  1022

/// <summary>
/// 完成文件发送
/// </summary>
#define WM_SEND_FILE_DONE				1101
/// <summary>
/// 文件发送失败
/// </summary>
#define WM_SEND_FILE_ERROR				1102
/// <summary>
/// 消息发送失败
/// </summary>
#define WM_SEND_MESSAGE_ERROR			1103
/// <summary>
/// 消息接收失败
/// </summary>
#define WM_RECEIVE_MESSAGE_ERROR       1104
/// <summary>
/// 在接收到单发或群发消息的时候触发。建议读取 history 列表并刷新
/// </summary>
#define WM_RECOMMEND_REFRESH_HISTORIES 1105
/// <summary>
/// 收到了一个文件时发送此消息。通过 LPARAM 传递一个 wchar_t* 作为文件名。
/// 在修改完 g_wsSaveFilePath 之后（处理完消息），就能自动保存了。
/// </summary>
#define WM_RECEIVE_FILE					1106
/// <summary>
/// 接收到 TIC 的时候触发，建议刷新 User 列表
/// </summary>
#define WM_RECEIVE_TIC					1111
/// <summary>
/// 接收到 TOC 的时候触发。可以不处理，在刷新的几秒钟结束之后统一刷新 User 列表
/// </summary>
#define WM_RECEIVE_TOC					1112
/// <summary>
/// 广播 TIC 失败。说明点击刷新按钮之后的操作失败了，这时候应该停止主窗口的刷新状态
/// </summary>
#define WM_BROADCAST_TIC_ERROR			1113
/// <summary>
/// 接收 TIC 的过程失败了，可以不处理。
/// </summary>
#define WM_RECEIVE_TIC_ERROR			1114

// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        135
#define _APS_NEXT_COMMAND_VALUE         32771
#define _APS_NEXT_CONTROL_VALUE         1034
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif
