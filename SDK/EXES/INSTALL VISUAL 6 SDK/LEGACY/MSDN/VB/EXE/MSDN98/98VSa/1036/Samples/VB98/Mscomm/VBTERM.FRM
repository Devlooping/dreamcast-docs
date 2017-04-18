VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.0#0"; "comdlg32.ocx"
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.0#0"; "mscomm32.ocx"
Object = "{831FDD16-0C5C-11d2-A9FC-0000F8754DA1}#1.0#0"; "mscomctl32.ocx"
Begin VB.Form frmTerminal
   Caption         =   "Terminal Visual Basic"
   ClientHeight    =   4935
   ClientLeft      =   2940
   ClientTop       =   2055
   ClientWidth     =   7155
   ForeColor       =   &H00000000&
   Icon            =   "vbterm.frx":0000
   LinkMode        =   1  'Source
   LinkTopic       =   "Form1"
   ScaleHeight     =   4935
   ScaleWidth      =   7155
   Begin VB.Timer Timer2 
      Enabled         =   0   'False
      Interval        =   2000
      Left            =   210
      Top             =   3645
   End
   Begin VB.TextBox txtTerm 
      Height          =   3690
      Left            =   1245
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   3
      Top             =   1140
      Width           =   5790
   End
   Begin MSComCtlLib.Toolbar tbrToolBar 
      Align           =   1  'Align Top
      Height          =   390
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   7155
      _ExtentX        =   12621
      _ExtentY        =   688
      ButtonWidth     =   609
      ButtonHeight    =   582
      ImageList       =   "ImageList1"
      BeginProperty Buttons {0713E452-850A-101B-AFC0-4210102A8DA7} 
         NumButtons      =   10
         BeginProperty Button1 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Style           =   3
            Value           =   1
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button2 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "OpenLogFile"
            Description     =   "Ouverture du fichier journal..."
            Object.ToolTipText     =   "Ouverture du fichier journal"
            ImageIndex      =   1
         EndProperty
         BeginProperty Button3 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Enabled         =   0   'False
            Key             =   "CloseLogFile"
            Description     =   "Fermeture du fichier journal"
            Object.ToolTipText     =   "Fermeture du fichier journal"
            ImageIndex      =   2
         EndProperty
         BeginProperty Button4 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Style           =   3
            Value           =   1
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button5 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "DialPhoneNumber"
            Description     =   "Compose le num�ro de t�l�phone..."
            Object.ToolTipText     =   "Compose le num�ro de t�l�phone"
            ImageIndex      =   3
         EndProperty
         BeginProperty Button6 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Enabled         =   0   'False
            Key             =   "HangUpPhone"
            Description     =   "Raccroche"
            Object.ToolTipText     =   "Raccroche"
            ImageIndex      =   4
         EndProperty
         BeginProperty Button7 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Style           =   3
            Value           =   1
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button8 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Key             =   "Properties"
            Description     =   "Propri�t�s..."
            Object.ToolTipText     =   "Propri�t�s"
            ImageIndex      =   5
         EndProperty
         BeginProperty Button9 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Style           =   3
            Value           =   1
            MixedState      =   -1  'True
         EndProperty
         BeginProperty Button10 {0713F354-850A-101B-AFC0-4210102A8DA7} 
            Enabled         =   0   'False
            Key             =   "TransmitTextFile"
            Description     =   "Transmet le fichier Texte..."
            Object.ToolTipText     =   "Transmet le fichier Texte"
            ImageIndex      =   6
         EndProperty
      EndProperty
      MouseIcon       =   "vbterm.frx":030A
      Begin VB.Frame Frame1 
         BorderStyle     =   0  'None
         Caption         =   "Frame1"
         Height          =   240
         Left            =   4000
         TabIndex        =   2
         Top             =   75
         Width           =   240
         Begin VB.Image imgConnected 
            Height          =   240
            Left            =   0
            Picture         =   "vbterm.frx":0326
            Stretch         =   -1  'True
            ToolTipText     =   "Bascule les ports"
            Top             =   0
            Width           =   240
         End
         Begin VB.Image imgNotConnected 
            Height          =   240
            Left            =   0
            Picture         =   "vbterm.frx":0470
            Stretch         =   -1  'True
            ToolTipText     =   "Bascule les ports"
            Top             =   0
            Width           =   240
         End
      End
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   165
      Top             =   1815
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   45
      Top             =   510
      _ExtentX        =   1005
      _ExtentY        =   1005
      DTREnable       =   -1  'True
      NullDiscard     =   -1  'True
      RThreshold      =   1
      RTSEnable       =   -1  'True
      SThreshold      =   1
      InputMode       =   1
   End
   Begin MSComDlg.CommonDialog OpenLog 
      Left            =   105
      Top             =   1170
      _ExtentX        =   847
      _ExtentY        =   847
      DefaultExt      =   "LOG"
      FileName        =   "Ouverture du fichier journal Communications"
      Filter          =   "Fichier journal (*.log)|*.log;"
      FilterIndex     =   501
      FontSize        =   9.02458e-38
   End
   Begin MSComCtlLib.StatusBar sbrStatus 
      Align           =   2  'Align Bottom
      Height          =   315
      Left            =   0
      TabIndex        =   0
      Top             =   4620
      Width           =   7155
      _ExtentX        =   12621
      _ExtentY        =   556
      BeginProperty Panels {0713E89E-850A-101B-AFC0-4210102A8DA7} 
         NumPanels       =   3
         BeginProperty Panel1 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   2
            Object.Width           =   2540
            MinWidth        =   2540
            Text            =   "�tat:"
            TextSave        =   "�tat:"
            Key             =   "Status"
            Object.ToolTipText     =   "�tat du port des communications"
         EndProperty
         BeginProperty Panel2 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   1
            Object.Width           =   8310
            MinWidth        =   2
            Text            =   "Param�tres:"
            TextSave        =   "Param�tres:"
            Key             =   "Settings"
            Object.ToolTipText     =   "Param�tre du port des communications"
         EndProperty
         BeginProperty Panel3 {0713E89F-850A-101B-AFC0-4210102A8DA7} 
            AutoSize        =   2
            Object.Width           =   1244
            MinWidth        =   1244
	    Key = "ConnectTime"
          
           Object.ToolTipText     =   "Heure de connexion"
         EndProperty
      EndProperty
      MouseIcon       =   "vbterm.frx":05BA
   End
   Begin MSComCtlLib.ImageList ImageList1 
      Left            =   165
      Top             =   2445
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      BeginProperty Images {0713E8C2-850A-101B-AFC0-4210102A8DA7} 
         NumListImages   =   6
         BeginProperty ListImage1 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "vbterm.frx":05D6
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "vbterm.frx":08F0
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "vbterm.frx":0C0A
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "vbterm.frx":0F24
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "vbterm.frx":123E
            Key             =   ""
         EndProperty
         BeginProperty ListImage6 {0713E8C3-850A-101B-AFC0-4210102A8DA7} 
            Picture         =   "vbterm.frx":1558
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&Fichier"
      Begin VB.Menu mnuOpenLog 
         Caption         =   "&Ouvrir un fichier journal..."
      End
      Begin VB.Menu mnuCloseLog 
         Caption         =   "F&ermer un fichier journal"
         Enabled         =   0   'False
      End
      Begin VB.Menu M3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuSendText 
         Caption         =   "&Transmettre un fichier Texte..."
         Enabled         =   0   'False
      End
      Begin VB.Menu Bar2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "&Quitter"
      End
   End
   Begin VB.Menu mnuPort 
      Caption         =   "Port de &communication"
      Begin VB.Menu mnuOpen 
         Caption         =   "&Ouvrir le port de communication"
      End
      Begin VB.Menu MBar1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuProperties 
         Caption         =   "Propri�t�s..."
      End
   End
   Begin VB.Menu mnuMSComm 
      Caption         =   "&MSComm"
      Begin VB.Menu mnuInputLen 
         Caption         =   "&InputLen..."
      End
      Begin VB.Menu mnuRThreshold 
         Caption         =   "&RThreshold..."
      End
      Begin VB.Menu mnuSThreshold 
         Caption         =   "&SThreshold..."
      End
      Begin VB.Menu mnuParRep 
         Caption         =   "P&arityReplace..."
      End
      Begin VB.Menu mnuDTREnable 
         Caption         =   "&DTREnable"
      End
      Begin VB.Menu Bar3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHCD 
         Caption         =   "&CDHolding..."
      End
      Begin VB.Menu mnuHCTS 
         Caption         =   "C&TSHolding..."
      End
      Begin VB.Menu mnuHDSR 
         Caption         =   "DSR&Holding..."
      End
   End
   Begin VB.Menu mnuCall 
      Caption         =   "&Appelle"
      Begin VB.Menu mnuDial 
         Caption         =   "&Compose le num�ro de t�l�phone..."
      End
      Begin VB.Menu mnuHangUp 
         Caption         =   "&Raccroche"
         Enabled         =   0   'False
      End
   End
End
Attribute VB_Name = "frmTerminal"
Attribute VB_Base = "0{3E2D207C-D67B-11CF-9BF3-00AA002FFD8F}"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_TemplateDerived = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'--------------------------------------------------
' VBTerm - Ceci est un programme de d�monstration pour
' le contr�le ActiveX de communications MSComm.
'
' Copyright (c) 1994, Crescent Software, Inc.
' par Don Malin et Carl Franklin.
'
' Mis � jour par Mike Maddox
'--------------------------------------------------
Option Explicit
                        
Dim Ret As Integer      ' Variable de travail sous forme d'entier.
Dim Temp As String      ' Variable de travail sous forme de cha�ne de caract�re.
Dim hLogFile As Integer ' Descripteur du fichier d'informations ouvert.
Dim StartTime As Date   ' Enregistre l'heure de d�but pour la minuterie du port.

Private Sub Form_Load()
    Dim CommPort As String, Handshaking As String, Settings As String
        
    On Error Resume Next
    
    ' D�finit la couleur par d�faut du terminal.
    txtTerm.SelLength = Len(txtTerm)
    txtTerm.SelText = ""
    txtTerm.ForeColor = vbBlue
       
    ' D�finit le titre.
    App.Title = "Terminal Visual Basic"
    
    ' D�finit les voyants de signalisation de l'�tat.
    imgNotConnected.ZOrder
       
    ' Centre la feuille.
    frmTerminal.Move (Screen.Width - Width) / 2, (Screen.Height - Height) / 2
    
    ' Charge les param�tres de la base de registres.
    
    Settings = GetSetting(App.Title, "Properties", "Settings", "") ' frmTerminal.MSComm1.Settings]\
    If Settings <> "" Then
        MSComm1.Settings = Settings
        If Err Then
            MsgBox Error$, 48
            Exit Sub
        End If
    End If
    
    CommPort = GetSetting(App.Title, "Properties", "CommPort", "") ' frmTerminal.MSComm1.CommPort
    If CommPort <> "" Then MSComm1.CommPort = CommPort
    
    Handshaking = GetSetting(App.Title, "Properties", "Handshaking", "") ' frmTerminal.MSComm1.Handshaking
    If Handshaking <> "" Then
        MSComm1.Handshaking = Handshaking
        If Err Then
            MsgBox Error$, 48
            Exit Sub
        End If
    End If
    
    Echo = GetSetting(App.Title, "Properties", "Echo", "") ' Echo
    On Error GoTo 0

End Sub

Private Sub Form_Resize()
   ' Redimensionne le contr�le Term (affichage).
   txtTerm.Move 0, tbrToolBar.Height, frmTerminal.ScaleWidth, frmTerminal.ScaleHeight - sbrStatus.Height - tbrToolBar.Height
   
   ' Positionne le voyant de signalisation de l'�tat.
   Frame1.Left = ScaleWidth - Frame1.Width * 1.5
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Dim Counter As Long

    If MSComm1.PortOpen Then
       ' Attend 10 secondes pour la transmission des donn�es.
       Counter = Timer + 10
       Do While MSComm1.OutBufferCount
          Ret = DoEvents()
          If Timer > Counter Then
             Select Case MsgBox("Impossible de transmettre les donn�es", 34)
                ' Annuler.
                Case 3
                   Cancel = True
                   Exit Sub
                ' R�essayer.
                Case 4
                   Counter = Timer + 10
                ' Ignorer.
                Case 5
                   Exit Do
             End Select
          End If
       Loop

       MSComm1.PortOpen = 0
    End If

    ' Si le fichier d'informations est ouvert, le vider et le fermer.
    If hLogFile Then mnuCloseLog_Click
    End
End Sub

Private Sub imgConnected_Click()
    ' Appelle la proc�dure mnuOpen_Click pour basculer entre la connexion et la d�connexion.
    Call mnuOpen_Click
End Sub

Private Sub imgNotConnected_Click()
    ' Appelle la proc�dure mnuOpen_Click pour basculer entre la connexion et la d�connexion.
    Call mnuOpen_Click
End Sub

Private Sub mnuCloseLog_Click()
    ' Ferme le fichier journal.
    Close hLogFile
    hLogFile = 0
    mnuOpenLog.Enabled = True
    tbrToolBar.Buttons("OpenLogFile").Enabled = True
    mnuCloseLog.Enabled = False
    tbrToolBar.Buttons("CloseLogFile").Enabled = False
    frmTerminal.Caption = "Terminal Visual Basic"
End Sub

Private Sub mnuDial_Click()
    On Local Error Resume Next
    Static Num As String
    
    Num = "1-206-936-6735" ' Ceci est le num�ro de t�l�phone MSDN.
    
    ' Obtient un num�ro de t�l�phone de l'utilisateur.
    Num = InputBox$("Veuillez composer un num�ro de t�l�phone:", "Num�ro compos�", Num)
    If Num = "" Then Exit Sub
    
    ' Ouvre le port de communication s'il n'�tait pas d�j� ouvert.
    If Not MSComm1.PortOpen Then
       mnuOpen_Click
       If Err Then Exit Sub
    End If
      
    ' Active le bouton Raccroche et les �l�ments de menu.
    mnuHangUp.Enabled = True
    tbrToolBar.Buttons("HangUpPhone").Enabled = True
              
    ' Compose le num�ro.
    MSComm1.Output = "ATDT" & Num & vbCrLf
    
    ' Lance la minuterie du port.
    StartTiming
End Sub

' Bascule la propri�t� DTREnabled.
Private Sub mnuDTREnable_Click()
    ' Bascule la propri�t� DTREnabled.
    MSComm1.DTREnable = Not MSComm1.DTREnable
    mnuDTREnable.Checked = MSComm1.DTREnable
End Sub


Private Sub mnuFileExit_Click()
    ' Utilise Form_Unload car il contient du code pour v�rifier la
    ' pr�sence de donn�es non transmises et l'ouverture du fichier
    ' journal.
    Form_Unload Ret
End Sub



' Bascule la propri�t� DTREnable pour raccrocher.
Private Sub mnuHangup_Click()
    On Error Resume Next
    
    MSComm1.Output = "ATH"      ' Envoie la cha�ne pour raccrocher.
    Ret = MSComm1.DTREnable     ' Enregistre le param�tre en cours.
    MSComm1.DTREnable = True    ' Active Terminal de donn�es pr�t (DTR).
    MSComm1.DTREnable = False   ' D�sactive Terminal de donn�es pr�t (DTR).
    MSComm1.DTREnable = Ret     ' Restaure l'ancien param�tre.
    mnuHangUp.Enabled = False
    tbrToolBar.Buttons("HangUpPhone").Enabled = False
    
    ' Si le port est encore ouvert, vous devez le fermer.
    If MSComm1.PortOpen Then MSComm1.PortOpen = False
    
    ' Informe l'utilisateur d'une erreur.
    If Err Then MsgBox Error$, 48
    
    mnuSendText.Enabled = False
    tbrToolBar.Buttons("TransmitTextFile").Enabled = False
    mnuHangUp.Enabled = False
    tbrToolBar.Buttons("HangUpPhone").Enabled = False
    mnuDial.Enabled = True
    tbrToolBar.Buttons("DialPhoneNumber").Enabled = True
    sbrStatus.Panels("Settings").Text = "Param�tres: "
    
    ' �teint le voyant de signalisation et d�coche le menu ouvert.
    mnuOpen.Checked = False
    imgNotConnected.ZOrder
            
    ' Arr�te la minuterie du port.
    StopTiming
    sbrStatus.Panels("Status").Text = "�tat: "
    On Error GoTo 0
End Sub

' Affiche la valeur de la propri�t� CDHolding.
Private Sub mnuHCD_Click()
    If MSComm1.CDHolding Then
        Temp = "True"
    Else
        Temp = "False"
    End If
    MsgBox "CDHolding = " + Temp
End Sub

' Affiche la valeur de la propri�t� CTSHolding.
Private Sub mnuHCTS_Click()
    If MSComm1.CTSHolding Then
        Temp = "True"
    Else
        Temp = "False"
    End If
    MsgBox "CTSHolding = " + Temp
End Sub

' Affiche la valeur de la propri�t� DSRHolding.
Private Sub mnuHDSR_Click()
    If MSComm1.DSRHolding Then
        Temp = "True"
    Else
        Temp = "False"
    End If
    MsgBox "DSRHolding = " + Temp
End Sub

' Cette proc�dure d�finit la propri�t� InputLen, qui d�termine
' le nombre d'octets de donn�es devant �tre lu chaque fois que
' la propri�t� Input est utilis�e pour lire des donn�es du
' tampon d'entr�e.
' Affecter la valeur 0 � la propri�t� InpuLen sp�cifie que le
' contenu entier du tampon doit �tre lu.
Private Sub mnuInputLen_Click()
    On Error Resume Next

    Temp = InputBox$("Entrez une nouvelle valeur pour InputLen:", "InputLen", Str$(MSComm1.InputLen))
    If Len(Temp) Then
        MSComm1.InputLen = Val(Temp)
        If Err Then MsgBox Error$, 48
    End If
End Sub

Private Sub mnuProperties_Click()
  ' Affiche la feuille des propri�t�s CommPort.
  frmProps.Show vbModal
  
End Sub

' Bascule l'�tat du port (ouvert ou ferm�).
Private Sub mnuOpen_Click()
    On Error Resume Next
    Dim OpenFlag

    MSComm1.PortOpen = Not MSComm1.PortOpen
    If Err Then MsgBox Error$, 48
    
    OpenFlag = MSComm1.PortOpen
    
    mnuOpen.Checked = OpenFlag
    mnuSendText.Enabled = OpenFlag
    tbrToolBar.Buttons("TransmitTextFile").Enabled = OpenFlag
        
    If MSComm1.PortOpen Then
        ' Active le bouton Composer et les �l�ments du menu.
        mnuDial.Enabled = True
        tbrToolBar.Buttons("DialPhoneNumber").Enabled = True
        
        ' Active le bouton Raccrocher et les �l�ments du menu.
        mnuHangUp.Enabled = True
        tbrToolBar.Buttons("HangUpPhone").Enabled = True
        
        imgConnected.ZOrder
        sbrStatus.Panels("Settings").Text = "Param�tres: " & MSComm1.Settings
        StartTiming
    Else
        ' Active le bouton Composer et les �l�ments du menu.
        mnuDial.Enabled = True
        tbrToolBar.Buttons("DialPhoneNumber").Enabled = True
        
        ' D�sactive le bouton Raccrocher et les �l�ments du menu.
        mnuHangUp.Enabled = False
        tbrToolBar.Buttons("HangUpPhone").Enabled = False
        
        imgNotConnected.ZOrder
        sbrStatus.Panels("Settings").Text = "Param�tres: "
        StopTiming
    End If
    
End Sub

Private Sub mnuOpenLog_Click()
   Dim replace
   On Error Resume Next
   OpenLog.Flags = cdlOFNHideReadOnly Or cdlOFNExplorer
   OpenLog.CancelError = True
      
   ' Obtient le nom du fichier journal de l'utilisateur.
   OpenLog.DialogTitle = "Ouvrir le fichier journal des communications"
   OpenLog.Filter = "Fichiers journal (*.LOG)|*.log|Tous les fichiers (*.*)|*.*"
   
   Do
      OpenLog.filename = ""
      OpenLog.ShowOpen
      If Err = cdlCancel Then Exit Sub
      Temp = OpenLog.filename

      ' Si le fichier existe d�j�, demande si l'utilisateur veut l'�craser ou l'ajouter au fichier existant.
Ret = Len(Dir$(Temp))
      If Err Then
         MsgBox Error$, 48
         Exit Sub
      End If
      If Ret Then
         replace = MsgBox("Replacer le fichier existant - " + Temp + "?", 35)
      Else
         replace = 0
      End If
   Loop While replace = 2

   ' L'utilisateur a cliqu� sur le bouton Oui; le fichier sera supprim�.
   If replace = 6 Then
      Kill Temp
      If Err Then
         MsgBox Error$, 48
         Exit Sub
      End If
   End If

   ' Ouvre le fichier journal.
   hLogFile = FreeFile
   Open Temp For Binary Access Write As hLogFile
   If Err Then
      MsgBox Error$, 48
      Close hLogFile
      hLogFile = 0
      Exit Sub
   Else
      ' Va � la fin du fichier pour ajouter de nouvelles donn�es.
      Seek hLogFile, LOF(hLogFile) + 1
   End If

   frmTerminal.Caption = "Terminal Visual Basic - " + OpenLog.FileTitle
   mnuOpenLog.Enabled = False
   tbrToolBar.Buttons("OpenLogFile").Enabled = False
   mnuCloseLog.Enabled = True
   tbrToolBar.Buttons("CloseLogFile").Enabled = True
End Sub

' Cette proc�dure d�finit la propri�t� ParityReplace contenant un
' caract�re qui remplace tous les caract�res incorrects re�us �
' cause d'une erreur de parit�.
Private Sub mnuParRep_Click()
    On Error Resume Next

    Temp = InputBox$("Entrez le caract�re de remplacement", "ParityReplace", frmTerminal.MSComm1.ParityReplace)
    frmTerminal.MSComm1.ParityReplace = Left$(Temp, 1)
    If Err Then MsgBox Error$, 48
End Sub

' Cette proc�dure d�finit la propri�t� RThreshold, qui d�termine
' le nombre d'octets de donn�es pouvant arriver dans le tampon
' d'entr�e, avant le d�clenchement de l'�v�nement OnComm et
' l'affectation de la valeur comEvReceive � la propri�t�
' CommEvent.
Private Sub mnuRThreshold_Click()
    On Error Resume Next
    
    Temp = InputBox$("Entrez une nouvelle valeur pour RThreshold:", "RThreshold", Str$(MSComm1.RThreshold))
    If Len(Temp) Then
        MSComm1.RThreshold = Val(Temp)
        If Err Then MsgBox Error$, 48
    End If

End Sub




' L'�v�nement OnComm est utilis� pour r�cup�rer des �v�nements et des erreurs de communication.
Private Static Sub MSComm1_OnComm()
    Dim EVMsg$
    Dim ERMsg$
    
    ' S�lectionne en fonction de la propri�t� CommEvent.
    Select Case MSComm1.CommEvent
        ' Messages d'�v�nement.
        Case comEvReceive
            Dim Buffer As Variant
            Buffer = MSComm1.Input
            Debug.Print "R�ception - " & StrConv(Buffer, vbUnicode)
            ShowData txtTerm, (StrConv(Buffer, vbUnicode))
        Case comEvSend
        Case comEvCTS
            EVMsg$ = "Changement de Pr�t � �mettre (CTS) d�tect�"
        Case comEvDSR
            EVMsg$ = "Changement de Modem pr�t (DSR) d�tect�"
        Case comEvCD
            EVMsg$ = "Changement de D�tection de porteuse (CD) d�tect�"
        Case comEvRing
            EVMsg$ = "Le t�l�phone sonne"
        Case comEvEOF
            EVMsg$ = "Fin de fichier d�tect�e"

        ' Messages d'erreur.
        Case comBreak
            ERMsg$ = "Pause re�ue"
        Case comCDTO
            ERMsg$ = "D�lai d'attente de D�tection de porteuse (CD) expir�"
        Case comCTSTO
            ERMsg$ = "D�lai d'attente de Pr�t � �mettre (CTS) expir�"
        Case comDCB
            ERMsg$ = "Erreur d'extraction de Bloc de contr�le de p�riph�rique (DCB)"
        Case comDSRTO
            ERMsg$ = "D�lai d'attente de Modem pr�t (DSR) expir�"
        Case comFrame
            ERMsg$ = "Erreur de cadrage"
        Case comOverrun
            ERMsg$ = "Erreur de cadence"
        Case comRxOver
            ERMsg$ = "D�passement du tampon de r�ception"
        Case comRxParity
            ERMsg$ = "Erreur de parit�"
        Case comTxFull
            ERMsg$ = "Tampon de transmission plein"
        Case Else
            ERMsg$ = "Erreur ou �v�nement inconnus"
    End Select
    
    If Len(EVMsg$) Then
        ' Affiche les messages d'erreur dans la barre d'�tat.
        sbrStatus.Panels("Status").Text = "�tat: " & EVMsg$
                
        ' Active la minuterie pour effacer le message contenu dans
        ' la barre d'�tat apr�s 2 secondes.
        Timer2.Enabled = True
        
    ElseIf Len(ERMsg$) Then
        ' Affiche les messages d'�v�nement dans la barre d'�tat.
        sbrStatus.Panels("Status").Text = "�tat: " & ERMsg$
        
        ' Affiche les messages d'erreur dans une bo�te de message.
        Beep
        Ret = MsgBox(ERMsg$, 1, "Cliquer sur Annuler pour quitter, sur OK pour ignorer.")
        
        ' Si l'utilisateur clique sur Annuler (2)...
        If Ret = 2 Then
            MSComm1.PortOpen = False    ' Ferme le port et quitte.
        End If
        
        ' Active la minuterie pour effacer le message contenu dans
        ' la barre d'�tat apr�s 2 secondes.
Timer2.Enabled = True
    End If
End Sub

Private Sub mnuSendText_Click()
   Dim hSend, BSize, LF&
   
   On Error Resume Next
   
   mnuSendText.Enabled = False
   tbrToolBar.Buttons("TransmitTextFile").Enabled = False
   
   ' Obtient le nom du fichier texte de l'utilisateur.
OpenLog.DialogTitle = "Transmettre un fichier Texte"
   OpenLog.Filter = "Fichiers Texte (*.TXT)|*.txt|Tous les fichiers (*.*)|*.*"
   Do
      OpenLog.CancelError = True
      OpenLog.filename = ""
      OpenLog.ShowOpen
      If Err = cdlCancel Then
        mnuSendText.Enabled = True
        tbrToolBar.Buttons("TransmitTextFile").Enabled = True
        Exit Sub
      End If
      Temp = OpenLog.filename

      ' Si le fichier n'existe pas, revient en arri�re.
      Ret = Len(Dir$(Temp))
      If Err Then
         MsgBox Error$, 48
         mnuSendText.Enabled = True
         tbrToolBar.Buttons("TransmitTextFile").Enabled = True
         Exit Sub
      End If
      If Ret Then
         Exit Do
      Else
         MsgBox Temp + " introuvable!", 48
      End If
   Loop

   ' Ouvre le fichier journal.
   hSend = FreeFile
   Open Temp For Binary Access Read As hSend
   If Err Then
      MsgBox Error$, 48
   Else
      ' Affiche la bo�te de dialogue Annuler.
CancelSend = False
      frmCancelSend.Label1.Caption = "Transmission d'un fichier texte - " + Temp
      frmCancelSend.Show
      
      ' Lit le fichier par blocs de la taille du tampon de transmission.
      BSize = MSComm1.OutBufferSize
      LF& = LOF(hSend)
      Do Until EOF(hSend) Or CancelSend
         ' Ne pas en lire autant � la fin.
         If LF& - Loc(hSend) <= BSize Then
            BSize = LF& - Loc(hSend) + 1
         End If
      
         ' Lit un bloc de donn�es.
         Temp = Space$(BSize)
         Get hSend, , Temp
      
         ' Transmet le bloc.
         MSComm1.Output = Temp
         If Err Then
            MsgBox Error$, 48
            Exit Do
         End If
      
         ' Attend que toutes les donn�es soient transmises.
         Do
            Ret = DoEvents()
         Loop Until MSComm1.OutBufferCount = 0 Or CancelSend
      Loop
   End If
   
   Close hSend
   mnuSendText.Enabled = True
   tbrToolBar.Buttons("TransmitTextFile").Enabled = True
   CancelSend = True
   frmCancelSend.Hide
End Sub

' Cette proc�dure d�finit la propri�t� SThreshold, qui d�termine
' le nombre d'octets de donn�es (au maximum) pouvant attendre dans
' le tampon de sortie, avant l'affectation de la valeur comEvSend
' � la propri�t� CommEvent et le d�clenchement de l'�v�nement
' OnComm.
Private Sub mnuSThreshold_Click()
    On Error Resume Next
    
    Temp = InputBox$("Entrez une nouvelle valeur pour SThreshold", "SThreshold", Str$(MSComm1.SThreshold))
    If Len(Temp) Then
        MSComm1.SThreshold = Val(Temp)
        If Err Then MsgBox Error$, 48
    End If
End Sub

' Cette proc�dure ajoute des donn�es dans la propri�t� Text du
' contr�le Term. Elle filtre �galement les caract�res de contr�le,
' comme le retour arri�re, le retour chariot, et les sauts de
' ligne, et inscrit les donn�es dans un fichier journal ouvert.
' Les caract�res de retour arri�re suppriment le caract�re de
' gauche dans la propri�t� Text ou dans la cha�ne pass�e. Les
' caract�res de saut de ligne sont ajout�s � tous les retours
' chariot. La taille de la propri�t� Text du contr�le Term est
' �galement contr�l�e de fa�on � ne pas exc�der les caract�res de
' la constante MAXTERMSIZE.
Private Static Sub ShowData(Term As Control, Data As String)
    On Error GoTo Handler
    Const MAXTERMSIZE = 16000
    Dim TermSize As Long, i
    
    ' V�rifie que le texte existant ne devient pas trop grand.
    TermSize = Len(Term.Text)
    If TermSize > MAXTERMSIZE Then
       Term.Text = Mid$(Term.Text, 4097)
       TermSize = Len(Term.Text)
    End If

    ' Pointe � la fin des donn�es du contr�le Term.
    Term.SelStart = TermSize

    ' Filtre/manipule les caract�res de retour arri�re.
    Do
       i = InStr(Data, Chr$(8))
       If i Then
          If i = 1 Then
             Term.SelStart = TermSize - 1
             Term.SelLength = 1
             Data = Mid$(Data, i + 1)
          Else
             Data = Left$(Data, i - 2) & Mid$(Data, i + 1)
          End If
       End If
    Loop While i

    ' �limine les sauts de ligne.
    Do
       i = InStr(Data, Chr$(10))
       If i Then
          Data = Left$(Data, i - 1) & Mid$(Data, i + 1)
       End If
    Loop While i

    ' V�rifie que tous les retours chariot ont un saut de ligne.
    i = 1
    Do
       i = InStr(i, Data, Chr$(13))
       If i Then
          Data = Left$(Data, i) & Chr$(10) & Mid$(Data, i + 1)
          i = i + 1
       End If
    Loop While i

    ' Ajoute les donn�es filtr�es � la propri�t� SelText.
    Term.SelText = Data
  
    ' Inscrit les donn�es dans le fichier sur demande.
    If hLogFile Then
       i = 2
       Do
          Err = 0
          Put hLogFile, , Data
          If Err Then
             i = MsgBox(Error$, 21)
             If i = 2 Then
                mnuCloseLog_Click
             End If
          End If
       Loop While i <> 2
    End If
    Term.SelStart = Len(Term.Text)
Exit Sub

Handler:
    MsgBox Error$
    Resume Next
End Sub

Private Sub Timer2_Timer()
sbrStatus.Panels("Status").Text = "�tat: "
Timer2.Enabled = False

End Sub

' Les touches de clavier r�cup�r�es ici sont envoy�es au contr�le
' MSComm qui les renvoie (en �cho) par l'interm�diaire de
' l'�v�nement OnComm (comEvReceive), et les affiche avec la
' proc�dure ShowData.
Private Sub txtTerm_KeyPress(KeyAscii As Integer)
    ' Si le port de communication est ouvert...
    If MSComm1.PortOpen Then
        ' Envoie les touches frapp�es vers le port de communication.
        MSComm1.Output = Chr$(KeyAscii)
        
        ' � moins que le mode �cho ne soit activ�,
        ' le contr�le texte n'a pas besoin d'afficher les touches.
        ' En g�n�ral, un modem renvoie un caract�re en retour.
        If Not Echo Then
            ' Place la position � la fin du terminal.
            txtTerm.SelStart = Len(txtTerm)
            KeyAscii = 0
        End If
    End If
     
End Sub


Private Sub tbrToolBar_ButtonClick(ByVal Button As MSComCtlLib.Button)
Select Case Button.Key
Case "OpenLogFile"
    Call mnuOpenLog_Click
Case "CloseLogFile"
    Call mnuCloseLog_Click
Case "DialPhoneNumber"
    Call mnuDial_Click
Case "HangUpPhone"
    Call mnuHangup_Click
Case "Properties"
    Call mnuProperties_Click
Case "TransmitTextFile"
    Call mnuSendText_Click
End Select
End Sub

Private Sub Timer1_Timer()
    ' Affiche l'heure de connexion
    sbrStatus.Panels("ConnectTime").Text = Format(Now - StartTime, "hh:mm:ss") & " "
End Sub
' Appelle cette fonction pour ex�cuter la minuterie de l'heure de connexion.
Private Sub StartTiming()
    StartTime = Now
    Timer1.Enabled = True
End Sub
' Appelle cette fonction pour arr�ter la minuterie.
Private Sub StopTiming()
    Timer1.Enabled = False
    sbrStatus.Panels("ConnectTime").Text = ""
End Sub
