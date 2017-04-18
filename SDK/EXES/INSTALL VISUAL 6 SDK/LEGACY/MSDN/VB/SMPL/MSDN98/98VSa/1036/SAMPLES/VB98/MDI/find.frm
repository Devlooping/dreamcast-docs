VERSION 5.00
Begin VB.Form frmFind 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Chercher"
   ClientHeight    =   1395
   ClientLeft      =   2655
   ClientTop       =   3585
   ClientWidth     =   4950
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1395
   ScaleWidth      =   4950
   Begin VB.Frame Frame1 
      Caption         =   "Direction"
      Height          =   612
      Left            =   1560
      TabIndex        =   3
      Top             =   720
      Width           =   2052
      Begin VB.OptionButton optDirection 
         Caption         =   "&Bas"
         Height          =   252
         Index           =   1
         Left            =   1080
         TabIndex        =   5
         ToolTipText     =   "Recherche jusqu'� la fin du document"
         Top             =   240
         Value           =   -1  'True
         Width           =   852
      End
      Begin VB.OptionButton optDirection 
         Caption         =   "&Haut"
         Height          =   252
         Index           =   0
         Left            =   240
         TabIndex        =   4
         ToolTipText     =   "Recherche jusqu'au d�but du document"
         Top             =   240
         Width           =   855
      End
   End
   Begin VB.CheckBox chkCase 
      Caption         =   "Respecter &la casse"
      Height          =   495
      Left            =   120
      TabIndex        =   2
      ToolTipText     =   "Respect des majuscules et des minuscules"
      Top             =   720
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   500
      Left            =   1200
      TabIndex        =   1
      ToolTipText     =   "Texte � chercher"
      Top             =   120
      Width           =   2415
   End
   Begin VB.CommandButton cmdcancel 
      Cancel          =   -1  'True
      Caption         =   "Annuler"
      Height          =   372
      Left            =   3720
      TabIndex        =   7
      ToolTipText     =   "Retourner au Bloc-notes"
      Top             =   600
      Width           =   1092
   End
   Begin VB.CommandButton cmdFind 
      Caption         =   "&Chercher"
      Default         =   -1  'True
      Height          =   372
      Left            =   3720
      TabIndex        =   6
      ToolTipText     =   "Commence la recherche"
      Top             =   120
      Width           =   1092
   End
   Begin VB.Label Label1 
      Caption         =   "&Rechercher:"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   1095
   End
End
Attribute VB_Name = "frmFind"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'*** Bo�te de dialogue Chercher pour rechercher du texte.***
'*** Cr�er pour l'exemple d'application Bloc-notes MDI.  ***
'*** Utilise: variables publiques gFindCase (bascule     ***
'*** l'option pour respecter la casse);                  ***
'*** gFindString (texte � chercher); gFindDirection      ***
'*** (bascule la direction de recherche); gFirstTime     ***
'*** (bascule le d�marrage � partir du texte).           ***
'***********************************************************

Option Explicit

Private Sub chkCase_Click()
    ' Affecte une valeur � la variable publique.
    gFindCase = chkCase.Value
End Sub

Private Sub cmdCancel_Click()
    ' Sauvegarde les valeurs dans les variables publiques.
    gFindString = Text1.Text
    gFindCase = chkCase.Value
    ' D�charge la bo�te de dialogue Chercher.
    Unload frmFind
End Sub

Private Sub cmdFind_Click()
    ' Affecte la cha�ne � la variable publique.
    gFindString = Text1.Text
    FindIt
End Sub

Private Sub Form_Load()
    ' D�sactive le bouton Chercher, aucun texte n'est encore � chercher.
    cmdFind.Enabled = False
    ' Lit la variable publique et d�finit le bouton d'option.
    optDirection(gFindDirection).Value = 1
End Sub

Private Sub optDirection_Click(index As Integer)
    ' Affecte une valeur � la variable publique.
    gFindDirection = index
End Sub

Private Sub Text1_Change()
    ' D�finit la variable publique.
    gFirstTime = True
    ' Si la zone de texte est vide, d�sactive le bouton Chercher.
    If Text1.Text = "" Then
        cmdFind.Enabled = False
    Else
        cmdFind.Enabled = True
    End If
End Sub

