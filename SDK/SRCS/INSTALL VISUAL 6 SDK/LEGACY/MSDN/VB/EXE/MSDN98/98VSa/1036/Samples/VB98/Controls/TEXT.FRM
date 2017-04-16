VERSION 5.00
Begin VB.Form frmText 
   Caption         =   "Zone de texte"
   ClientHeight    =   4455
   ClientLeft      =   930
   ClientTop       =   1290
   ClientWidth     =   6915
   LinkTopic       =   "Form1"
   ScaleHeight     =   4455
   ScaleWidth      =   6915
   Begin VB.CommandButton cmdReset 
      Caption         =   "&R�initialiser"
      Height          =   495
      Left            =   1560
      TabIndex        =   2
      Top             =   3720
      Width           =   1215
   End
   Begin VB.CommandButton cmdClose 
      Cancel          =   -1  'True
      Caption         =   "&Fermer"
      Height          =   495
      Left            =   4800
      TabIndex        =   3
      Top             =   3720
      Width           =   1215
   End
   Begin VB.Frame fraInsert 
      Caption         =   "D�finit le point d'insertion"
      Height          =   2295
      Left            =   120
      TabIndex        =   5
      Top             =   1200
      Width           =   3735
      Begin VB.OptionButton optText 
         Caption         =   "Insertion de &texte"
         Height          =   255
         Left            =   360
         TabIndex        =   9
         Top             =   1800
         Width           =   2055
      End
      Begin VB.OptionButton optSelect 
         Caption         =   "&S�lection de tout le texte"
         Height          =   255
         Left            =   360
         TabIndex        =   8
         Top             =   1440
         Width           =   2055
      End
      Begin VB.OptionButton optInsert 
         Caption         =   "Point d'&insertion apr�s le 5e caract�re"
         Height          =   255
         Left            =   360
         TabIndex        =   7
         Top             =   1080
         Width           =   3255
      End
      Begin VB.OptionButton optEnd 
         Caption         =   "&Point d'insertion � la fin"
         Height          =   255
         Left            =   360
         TabIndex        =   6
         Top             =   720
         Width           =   1935
      End
      Begin VB.OptionButton optDefault 
         Caption         =   "&Valeurs par d�faut"
         Height          =   255
         Left            =   360
         TabIndex        =   1
         Top             =   360
         Value           =   -1  'True
         Width           =   1815
      End
   End
   Begin VB.TextBox txtDisplay 
      Height          =   285
      HideSelection   =   0   'False
      Left            =   120
      TabIndex        =   0
      Text            =   "La valeur True est affect�e � la propri�t� Multiline dans cet exemple."
      Top             =   480
      Width           =   3735
   End
   Begin VB.TextBox txtMulti 
      Height          =   735
      Left            =   4080
      MultiLine       =   -1  'True
      TabIndex        =   4
      TabStop         =   0   'False
      Text            =   "text.frx":0000
      Top             =   360
      Width           =   2655
   End
   Begin VB.Label lblHelp 
      Caption         =   "S�lectionnez une option pour voir ses effets sur la premi�re zone de texte."
      Height          =   735
      Left            =   4320
      TabIndex        =   10
      Top             =   2160
      Width           =   2295
   End
End
Attribute VB_Name = "frmText"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False




Private Sub cmdClose_Click()
    Unload Me   ' D�charge la feuille.
End Sub

Private Sub cmdReset_Click()
    ' Restaure le texte original.
    txtDisplay.Text = " La valeur True est affect�e � la propri�t� Multiline dans cet exemple "
    ' R�initialise le groupe d'options avec le bouton par d�faut.
    optDefault.Value = True
End Sub

Private Sub optDefault_Click()
    ' Place le point d'insertion au d�but.
    txtDisplay.SelStart = 0
    
    ' Attribue le focus � la zone de texte. Ainsi, vous pouvez
    ' voir le r�sultat de vos valeurs.
    txtDisplay.SetFocus
End Sub

Private Sub optEnd_Click()
    ' Trouve la longueur de la cha�ne et place le point d'insertion � la fin.
    txtDisplay.SelStart = Len(txtDisplay.Text)
    
    ' Attribue le focus � la zone de texte. Ainsi, vous pouvez
    ' voir le r�sultat de vos valeurs.
    txtDisplay.SetFocus
End Sub


Private Sub optInsert_Click()
    ' place le point d'insertion apr�s le cinqui�me caract�re.
    txtDisplay.SelStart = 5
        
    ' Attribue le focus � la zone de texte. Ainsi, vous pouvez
    ' voir le r�sultat de vos valeurs.
    txtDisplay.SetFocus
End Sub


Private Sub optSelect_Click()
    ' place le point d'insertion au d�but.
    txtDisplay.SelStart = 0
    ' Trouve la longueur de la cha�ne et s�lectionne ce nombre de caract�res.
    txtDisplay.SelLength = Len(txtDisplay.Text)

    ' Attribue le focus � la zone de texte. Ainsi, vous pouvez
    ' voir le r�sultat de vos valeurs.
    txtDisplay.SetFocus
End Sub


Private Sub optText_Click()
    ' Ins�re "NOUVELLE CHA�NE" au point d'insertion, si le texte est s�lectionn�, il sera remplac�.
txtDisplay.SelText = " NOUVELLE CHA�NE"
    
    ' Attribue le focus � la zone de texte. Ainsi, vous pouvez
    ' voir le r�sultat de vos valeurs.
    txtDisplay.SetFocus
End Sub

