VERSION 5.00
Begin VB.Form frmSQL 
   Caption         =   "Instruction SQL"
   ClientHeight    =   2880
   ClientLeft      =   3690
   ClientTop       =   1575
   ClientWidth     =   5130
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   HelpContextID   =   2016144
   Icon            =   "SQL.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MDIChild        =   -1  'True
   ScaleHeight     =   2863.353
   ScaleMode       =   0  'User
   ScaleWidth      =   5147.588
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdSaveQueryDef 
      Caption         =   "Enregi&strer"
      Height          =   375
      Left            =   3375
      MaskColor       =   &H00000000&
      TabIndex        =   3
      Top             =   30
      Visible         =   0   'False
      Width           =   1695
   End
   Begin VB.CommandButton cmdExecuteSQL 
      Caption         =   "&Ex�cuter"
      Default         =   -1  'True
      Enabled         =   0   'False
      Height          =   375
      Left            =   15
      MaskColor       =   &H00000000&
      TabIndex        =   1
      Top             =   30
      Width           =   1575
   End
   Begin VB.CommandButton cmdClearSQL 
      Caption         =   "E&ffacer"
      Height          =   375
      Left            =   1695
      MaskColor       =   &H00000000&
      TabIndex        =   2
      Top             =   30
      Width           =   1575
   End
   Begin VB.TextBox txtSQLStatement 
      Height          =   2175
      Left            =   15
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   435
      Width           =   5055
   End
End
Attribute VB_Name = "frmSQL"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
'>>>>>>>>>>>>>>>>>>>>>>>>
Const FORMCAPTION = "Instruction SQL"
Const BUTTON1 = "&Ex�cuter"
Const BUTTON2 = "E&ffacer"
Const BUTTON3 = "Enregi&strer"
Const MSG1 = "Mise � jour"
Const MSG2 = "Entrez le nom de la requ�te:"
Const MSG3 = "S'agit-il d'une requ�te SQL Direct?"
Const MSG4 = "Entrez la valeur de la propri�t� Connect:"
Const MSG5 = "La ligne de la requ�te renvoie-t-elle une valeur?"
'>>>>>>>>>>>>>>>>>>>>>>>>


Private Sub cmdClearSQL_Click()
  txtSQLStatement.Text = vbNullString
  txtSQLStatement.SetFocus
End Sub

Private Sub cmdSaveQueryDef_Click()
  On Error GoTo SQDErr

  Dim sQueryName As String
  Dim sTmp As String
  Dim qdNew As QueryDef

  If Not gnodDBNode Is Nothing Then
    If gnodDBNode.Tag = QUERY_STR Then
      ' Une requ�te est s�lectionn�e; l'utilisateur voudra mettre � jour sa requ�te SQL
      If MsgBox(MSG1 & " '" & gnodDBNode.Text & "'?", vbYesNo + vbQuestion) = vbYes Then
        ' Stocke la requ�te SQL � partir de la fen�tre SQL
        ' dans la requ�te s�lectionn�e en cours.
        gdbCurrentDB.QueryDefs(gnodDBNode.Text).SQL = Me.txtSQLStatement.Text
        Exit Sub
      End If
    End If
  End If
  
  ' Aucune requ�te n'est s�lectionn�e ou l'utilisateur n'a pas
  ' demand� de mise � jour pour la requ�te en cours; il convient
  ' de taper un nouveau nom.
  sQueryName = InputBox(MSG2)
  If Len(sQueryName) = 0 Then Exit Sub
  
  ' Recherche un duplicata et quitte si l'utilisateur ne r��crit pas par dessus.
  If DupeTableName(sQueryName) Then
    Exit Sub
  End If
    
  ' Ajoute la nouvelle requ�te.
  Set qdNew = gdbCurrentDB.CreateQueryDef(sQueryName)
  ' Demande de taper une requ�te SQL Passthrough ou SQL Direct.
  If MsgBox(MSG3, vbYesNo + vbQuestion + vbDefaultButton2) = vbYes Then
    sTmp = InputBox(MSG4)
    If Len(sTmp) > 0 Then
      qdNew.Connect = sTmp
      If MsgBox(MSG5, vbYesNo + vbQuestion) = vbNo Then
        qdNew.ReturnsRecords = False
      End If
    End If
  End If
  qdNew.SQL = txtSQLStatement.Text
  
  gdbCurrentDB.QueryDefs.Refresh
  RefreshTables Nothing

  Exit Sub

SQDErr:
  ShowError
End Sub

Private Sub Form_Unload(Cancel As Integer)
  ' Enregistre le texte SQL uniquement si aucun retour chariot
  ' n'est pr�sent, car ils ne sont pas g�r�s dans le fichier INI.
  ' If InStr(frmSQL.txtSQLStatement.Text, Chr(13)) = 0 Then
    SaveSetting APP_CATEGORY, APPNAME, "SQLStatement", frmSQL.txtSQLStatement.Text
  ' End If
  If frmSQL.WindowState = vbNormal Then
    SaveSetting APP_CATEGORY, APPNAME, "SQLWindowTop", frmSQL.Top
    SaveSetting APP_CATEGORY, APPNAME, "SQLWindowLeft", frmSQL.Left
    SaveSetting APP_CATEGORY, APPNAME, "SQLWindowWidth", frmSQL.Width
    SaveSetting APP_CATEGORY, APPNAME, "SQLWindowHeight", frmSQL.Height
  End If
End Sub

Private Sub txtSQLStatement_Change()
  cmdExecuteSQL.Enabled = Len(txtSQLStatement.Text) > 0
End Sub

Private Sub cmdExecuteSQL_Click()
  If Len(txtSQLStatement.Text) = 0 Then Exit Sub

  OpenQuery txtSQLStatement.Text, True
End Sub

Private Sub Form_KeyUp(KeyCode As Integer, Shift As Integer)
  If KeyCode = vbKeyF1 And Shift = 0 Then
    DisplayTopic 2016144
  End If
End Sub

Private Sub Form_Load()
  Me.Caption = FORMCAPTION
  cmdExecuteSQL.Caption = BUTTON1
  cmdClearSQL.Caption = BUTTON2
  cmdSaveQueryDef.Caption = BUTTON3
  
  txtSQLStatement.Text = GetRegistryString("SQLStatement", vbNullString)
 
  Me.Height = Val(GetRegistryString("SQLWindowHeight", "3000"))
  Me.Width = Val(GetRegistryString("SQLWindowWidth", "5370"))
  Me.Top = Val(GetRegistryString("SQLWindowTop", "0"))
  Me.Left = Val(GetRegistryString("SQLWindowLeft", "3850"))

End Sub

Private Sub Form_Resize()
  On Error Resume Next

  If WindowState <> vbMinimized Then
    txtSQLStatement.Width = Me.ScaleWidth - (txtSQLStatement.Left * 2)
    txtSQLStatement.Height = Me.ScaleHeight - (txtSQLStatement.Top + 50)
  End If
End Sub

Private Sub txtSQLStatement_DragDrop(Source As Control, x As Single, y As Single)
  If Source = frmDatabase.tvDatabase Then
    frmSQL.txtSQLStatement.Text = gdbCurrentDB.QueryDefs(gnodDBNode.Text).SQL
  End If
End Sub
