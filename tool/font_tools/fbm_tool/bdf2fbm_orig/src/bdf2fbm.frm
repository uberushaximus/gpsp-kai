VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmbdf2fbm 
   BorderStyle     =   1  'å≈íË(é¿ê¸)
   Caption         =   "bdf2fbm"
   ClientHeight    =   1455
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4965
   BeginProperty Font 
      Name            =   "ÇlÇr ÉSÉVÉbÉN"
      Size            =   9
      Charset         =   128
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   97
   ScaleMode       =   3  'Àﬂ∏æŸ
   ScaleWidth      =   331
   StartUpPosition =   3  'Windows ÇÃä˘íËíl
   Begin VB.CommandButton cmdBdfFile 
      Caption         =   "BDF"
      Height          =   255
      Left            =   3840
      TabIndex        =   0
      Top             =   120
      Width           =   855
   End
   Begin VB.TextBox txtBdfFile 
      Height          =   270
      Left            =   240
      Locked          =   -1  'True
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   120
      Width           =   3495
   End
   Begin VB.CommandButton cmdConvert 
      Caption         =   "Convert"
      Height          =   735
      Left            =   240
      TabIndex        =   1
      Top             =   600
      Width           =   4455
   End
   Begin MSComDlg.CommonDialog cdlgBdfFile 
      Left            =   4440
      Top             =   0
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      Filter          =   "Bdf files (*.bdf)|*.bdf|All Files (*.*)|*.*"
   End
End
Attribute VB_Name = "frmbdf2fbm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Const cTAG_WxH = "FONTBOUNDINGBOX "
Const cTAG_CharSet = "CHARSET_REGISTRY "
Const cTAG_Default = "DEFAULT_CHAR "
Const cTAG_Font = "ENCODING "


Private Type typFont
    bytWidth        As Byte
    bytFont()       As Byte
End Type


Private Type typFontMap
    bytStart(1)     As Byte
    bytEnd(1)       As Byte
    bytDistance(1)  As Byte
End Type


Private Type typFontControl
    bytFontCnt(1)   As Byte
    bytMapCnt(1)    As Byte
    bytDefault(1)   As Byte
    bytWidth        As Byte
    bytHeight       As Byte
    bytByteParChar  As Byte
End Type


Private Sub cmdBdfFile_Click()
    Dim strWork     As String
    Dim intCnt      As Integer


    On Error GoTo Cancel_Label

    cdlgBdfFile.ShowOpen
    txtBdfFile.Text = cdlgBdfFile.FileName

    On Error Resume Next

Cancel_Label:
    DoEvents
End Sub


Private Sub cmdConvert_Click()
    Dim strFontFile     As String
    Dim strBuf          As String
    Dim strCharset      As String
    Dim usrFont()       As typFont
    Dim lngCode()       As Long
    Dim usrFontControl  As typFontControl
    Dim usrFontMap()    As typFontMap
    Dim lngFontCnt      As Long
    Dim lngMapCnt       As Long
    Dim intWidth        As Integer
    Dim intHeight       As Integer
    Dim lngDefaultTemp  As Long
    Dim lngDefault      As Long
    Dim lngCnt          As Long


    If txtBdfFile.Text = "" Then Exit Sub

    strBuf = txtBdfFile.Text

    For lngCnt = Len(strBuf) To 1 Step -1
        If Mid(strBuf, lngCnt, 1) = "." Then
            strBuf = Left(strBuf, lngCnt - 1)
            Exit For
        ElseIf Mid(strBuf, lngCnt, 1) = "\" Then
            Exit For
        End If
    Next

    strFontFile = strBuf & ".fbm"

    Me.MousePointer = vbHourglass
    cmdConvert.Caption = "Processing ..."

    lngFontCnt = 0
    lngMapCnt = 0
    intWidth = 0
    intHeight = 0
    lngDefaultTemp = 0
    lngDefault = 0

    Open txtBdfFile.Text For Input Access Read As #1

    Do While Not EOF(1)
        fgets 1, strBuf

        If cTAG_WxH = Left(strBuf, Len(cTAG_WxH)) Then
            strBuf = Mid(strBuf, Len(cTAG_WxH) + 1, Len(strBuf) - Len(cTAG_WxH) + 1)
            intWidth = Left(strBuf, InStr(1, strBuf, " ") - 1)
            strBuf = Right(strBuf, Len(strBuf) - InStr(1, strBuf, " "))
            intHeight = Left(strBuf, InStr(1, strBuf, " ") - 1)
            Debug.Print "WIDTHxHEIGHT:" & intWidth & "x" & intHeight
        ElseIf cTAG_CharSet = Left(strBuf, Len(cTAG_CharSet)) Then
            strBuf = Mid(strBuf, Len(cTAG_CharSet) + 1, Len(strBuf) - Len(cTAG_CharSet) + 1)
            strBuf = Mid(strBuf, InStr(1, strBuf, """") + 1)
            strCharset = Left(strBuf, InStr(1, strBuf, """") - 1)
            
            If fncCharsetCheck(strCharset) < 0 Then
                MsgBox "CHARSET is not support! [" & strCharset & "]", vbExclamation + vbOKOnly, "Error"
                Close #1
                GoTo End_Label
            End If

            Debug.Print "CODESET:" & strCharset
        ElseIf cTAG_Default = Left(strBuf, Len(cTAG_Default)) Then
            strBuf = Mid(strBuf, Len(cTAG_Default) + 1, Len(strBuf) - Len(cTAG_Default) + 1)
            lngDefaultTemp = fncCharCodeConv(strCharset, CLng(strBuf))
            Debug.Print "DEFAULT_CHAR:" & lngDefaultTemp
        ElseIf cTAG_Font = Left(strBuf, Len(cTAG_Font)) Then
            If intWidth = 0 Or intHeight = 0 Then Exit Do

            ReDim Preserve usrFont(lngFontCnt)
            ReDim Preserve lngCode(lngFontCnt)
            strBuf = Mid(strBuf, Len(cTAG_Font) + 1, Len(strBuf) - Len(cTAG_Font) + 1)
            lngCode(lngFontCnt) = fncCharCodeConv(strCharset, CLng(strBuf))
            ReDim usrFont(lngFontCnt).bytFont(Int(intWidth / 8 + 0.99999) * intHeight - 1)

            subMkFont 1, usrFont(lngFontCnt), Int(intWidth / 8 + 0.99999)
            lngFontCnt = lngFontCnt + 1
        End If
    Loop

    Close #1

    If intWidth = 0 Or intHeight = 0 Or lngFontCnt = 0 Then
        MsgBox "File is not support !", vbExclamation + vbOKOnly, "Error"
        GoTo End_Label
    End If

    lngMapCnt = 0
    ReDim usrFontMap(0)

    subShort2Byte lngCode(0), usrFontMap(0).bytStart
    subShort2Byte lngCode(0), usrFontMap(0).bytEnd
    subShort2Byte lngCode(0) - 0, usrFontMap(0).bytDistance
    If lngCode(0) = lngDefaultTemp Then lngDefault = 0

    For lngCnt = 1 To UBound(usrFont)
        If lngCode(lngCnt) <> lngCode(lngCnt - 1) + 1 Then
            lngMapCnt = lngMapCnt + 1
            ReDim Preserve usrFontMap(lngMapCnt)

            With usrFontMap(lngMapCnt)
                subShort2Byte lngCode(lngCnt), .bytStart
                subShort2Byte lngCode(lngCnt) - lngCnt, .bytDistance
            End With
        End If

        If lngCode(lngCnt) = lngDefaultTemp Then lngDefault = lngCnt
        subShort2Byte lngCode(lngCnt), usrFontMap(lngMapCnt).bytEnd
    Next

    lngMapCnt = lngMapCnt + 1

    If lngDefault > UBound(usrFont) Then lngDefault = 0

    With usrFontControl
        subShort2Byte lngFontCnt, .bytFontCnt
        subShort2Byte lngMapCnt, .bytMapCnt
        subShort2Byte lngDefault, .bytDefault
        .bytWidth = CByte(intWidth)
        .bytHeight = CByte(intHeight)
        .bytByteParChar = CByte(Int(intWidth / 8 + 0.99999) * intHeight)
    End With
    
    For lngCnt = 1 To UBound(usrFontMap)
        If usrFontMap(lngCnt - 1).bytStart(1) > usrFontMap(lngCnt).bytStart(1) Or _
           (usrFontMap(lngCnt - 1).bytStart(1) = usrFontMap(lngCnt).bytStart(1) And _
            usrFontMap(lngCnt - 1).bytStart(0) > usrFontMap(lngCnt).bytStart(0)) Then
            Debug.Print "ERROR:" & lngCnt
        End If
    Next

    Open strFontFile For Binary Access Write As #1

    Put #1, , usrFontControl
    Put #1, , usrFontMap

    For lngCnt = 0 To UBound(usrFont)
        Put #1, , usrFont(lngCnt).bytWidth
        Put #1, , usrFont(lngCnt).bytFont
    Next

    Close #1


End_Label:
    Me.MousePointer = vbNormal
    cmdConvert.Caption = "Convert"
End Sub


' éËî≤Ç´
Private Function fncCharsetCheck(strCharset As String) As Integer
    If StrConv(Left(strCharset, Len("ISO8859")), vbUpperCase) = "ISO8859" Then
        fncCharsetCheck = 0
    ElseIf StrConv(Left(strCharset, Len("JISX0201")), vbUpperCase) = "JISX0201" Then
        fncCharsetCheck = 1
    ElseIf StrConv(Left(strCharset, Len("JISX0208")), vbUpperCase) = "JISX0208" Then
        fncCharsetCheck = 2
    Else
        fncCharsetCheck = -1
    End If
End Function
               

Private Function fncCharCodeConv(strCharset As String, lngCode As Long) As Long
    Select Case fncCharsetCheck(strCharset)
        Case 2  ' JISX0208
            fncCharCodeConv = fncJis2Sjis(lngCode)

        Case Else
            fncCharCodeConv = lngCode
    End Select
End Function


Private Function fncJis2Sjis(lngCode As Long) As Long
    Dim lngC1       As Long
    Dim lngC2       As Long


    lngC1 = Int(lngCode / 256)
    lngC2 = Int(lngCode Mod 256)

    If lngC1 = 0 Then
        fncJis2Sjis = lngCode
        Exit Function
    End If

    If (lngC1 Mod 2) = 0 Then
        lngC2 = lngC2 + &H7D
    Else
        lngC2 = lngC2 + &H1F
    End If

    If lngC2 > &H7E Then lngC2 = lngC2 + 1

    If lngC1 < &H5F Then
        lngC1 = Int((lngC1 + 1) / 2) + &H70
    Else
        lngC1 = Int((lngC1 + 1) / 2) + &HB0
    End If

    fncJis2Sjis = lngC1 * 256 + lngC2
End Function


Private Sub subShort2Byte(lngDat As Long, bytDat() As Byte)
    bytDat(0) = CByte(lngDat Mod 256)
    bytDat(1) = CByte(Int(lngDat / 256))
End Sub


Private Sub subMkFont(fp As Integer, usrFont As typFont, intWidth As Integer)
    Dim strBuf      As String
    Dim strWork     As String
    Dim intLen      As Integer
    Dim intCnt      As Integer
    Dim intPixel    As Integer


    Do While Not EOF(1)
        fgets 1, strBuf

        If "DWIDTH " = Left(strBuf, Len("DWIDTH ")) Then
            strWork = Mid(strBuf, Len("DWIDTH ") + 1, InStr(Len("DWIDTH "), strBuf, " ") - Len("DWIDTH ") + 2)
            usrFont.bytWidth = CByte(strWork)
        ElseIf "BITMAP" = Left(strBuf, Len("BITMAP")) Then
            Do While Not EOF(1)
                fgets 1, strBuf

                If "ENDCHAR" = Left(strBuf, Len("ENDCHAR")) Then Exit Sub

                intLen = Int(Len(strBuf) / 2)

                With usrFont
                    For intCnt = 0 To intWidth - 1
                        .bytFont(intPixel + intCnt) = fncHexStr2Byte(Mid(strBuf, intCnt * 2 + 1, 2))
                    Next

                    intPixel = intPixel + intWidth
                End With
            Loop
        End If
    Loop
End Sub


Private Function fncHexStr2Byte(strHexStr As String) As Byte
    fncHexStr2Byte = 0
    fncHexStr2Byte = fncHexChar2Int(Left(strHexStr, 1)) * 16 + fncHexChar2Int(Right(strHexStr, 1))
End Function


Private Function fncHexChar2Int(strHexChar As String) As Integer
    fncHexChar2Int = 0

    If strHexChar >= "0" And strHexChar <= "9" Then
        fncHexChar2Int = Int(strHexChar)
    ElseIf StrConv(strHexChar, vbUpperCase) >= "A" And StrConv(strHexChar, vbUpperCase) <= "F" Then
        fncHexChar2Int = Asc(StrConv(strHexChar, vbUpperCase)) - Asc("A") + 10
    End If
End Function


Private Function fgets(fp As Integer, strBuf As String) As Integer
    Static bln0x0D  As Boolean
    Static bln0x0A  As Boolean
    Dim strChar     As String


    fgets = 0
    strBuf = ""

    Do While Not EOF(1)
        strChar = Input(1, #1)

        If Asc(strChar) = &HD Then
            If bln0x0A Then
                bln0x0A = False
            Else
                bln0x0D = True
                Exit Do
            End If
        ElseIf Asc(strChar) = &HA Then
            If bln0x0D Then
                bln0x0D = False
            Else
                bln0x0A = True
                Exit Do
            End If
        ElseIf Asc(strChar) = 0 Then
            bln0x0A = False
            bln0x0D = False
            Exit Do
        Else
            bln0x0A = False
            bln0x0D = False
            strBuf = strBuf & strChar
            fgets = fgets + 1
        End If
    Loop
End Function
