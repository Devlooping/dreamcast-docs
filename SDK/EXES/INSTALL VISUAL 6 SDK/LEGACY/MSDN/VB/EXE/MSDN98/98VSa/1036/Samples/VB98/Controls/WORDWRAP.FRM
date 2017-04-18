VERSION 5.00
Begin VB.Form frmWordWrap 
   Caption         =   "AutoSize et WordWrap"
   ClientHeight    =   4320
   ClientLeft      =   1110
   ClientTop       =   1665
   ClientWidth     =   5760
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   ScaleHeight     =   4320
   ScaleWidth      =   5760
   Begin VB.CommandButton cmdCycle 
      Caption         =   "C&ycle"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   480
      TabIndex        =   2
      Top             =   2520
      Width           =   1215
   End
   Begin VB.CheckBox chkAutoSize 
      Caption         =   "&AutoSize"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   3840
      TabIndex        =   0
      Top             =   1080
      Width           =   1215
   End
   Begin VB.CheckBox chkWordWrap 
      Caption         =   "&WordWrap"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   3840
      TabIndex        =   1
      Top             =   1680
      Width           =   1575
   End
   Begin VB.CommandButton cmdClose 
      Cancel          =   -1  'True
      Caption         =   "&Fermer"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   3840
      TabIndex        =   3
      Top             =   2520
      Width           =   1215
   End
   Begin VB.Label lblHelp 
      BackColor       =   &H00C0C0C0&
      Caption         =   "Acivez les cases � cocher AutoSize et/ou WordWrap pour voir leurs effets sur le contr�le Label."
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   480
      TabIndex        =   5
      Top             =   3360
      Width           =   2775
   End
   Begin VB.Label lblDisplay 
      BorderStyle     =   1  'Fixed Single
      Caption         =   "D�monstration des propri�t�s AutoSize et WordWrap."
      Height          =   255
      Left            =   480
      TabIndex        =   4
      Top             =   480
      Width           =   1815
   End
End
Attribute VB_Name = "frmWordWrap"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Sub Display()
    ' Cette proc�dure change les caract�ristiques de l'affichage du contr�le Label.

    ' R�initialise l'exemple.
    lblDisplay.AutoSize = False
    lblDisplay.WordWrap = False
    lblDisplay.Width = 1695
    lblDisplay.Height = 225
    ' V�rifie pour WordWrap et Autosize.
    If chkWordWrap.Value = 1 Then
        lblDisplay.WordWrap = True
    End If
    If chkAutoSize.Value = 1 Then
        lblDisplay.AutoSize = True
    End If
End Sub

Private Sub chkAutoSize_Click()
    ' Appelle la proc�dure Display.
    Call Display
End Sub


Private Sub chkWordWrap_Click()
    ' Appelle la proc�dure Display.
    Call Display
End Sub


Private Sub cmdClose_Click()
   Unload Me    ' D�charge la feuille.
End Sub

Private Sub cmdCycle_Click()
    ' Permet de faire d�filer une � une les quatre combinaisons possibles.
    
    ' Aucune case � cocher n'est activ�e.
    If chkAutoSize.Value = 0 And _
     chkWordWrap.Value = 0 Then
        ' Active la case � cocher AutoSize.
        chkAutoSize.Value = 1
    ' Les deux cases � cocher sont activ�es.
    ElseIf chkAutoSize.Value = 1 And _
     chkWordWrap.Value = 1 Then
        ' D�sactive la case � cocher AutoSize et active la case � cocher WordWrap.
        chkAutoSize.Value = 0
        chkWordWrap.Value = 1
    ' Seule la case � cocher WordWrap est activ�e.
    ElseIf chkAutoSize.Value = 0 And _
     chkWordWrap.Value = 1 Then
        ' D�sactive les deux cases � cocher.
        chkAutoSize.Value = 0
        chkWordWrap.Value = 0
    ' Seule la case � cocher AutoSize est activ�e.
    Else
        ' Active la case � cocher WordWrap, la case � cocher
        ' AutoSize �tant d�j� activ�e.
        chkWordWrap.Value = 1
    End If
End Sub


