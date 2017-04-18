VERSION 5.00
Begin VB.Form frmSticks 
   Caption         =   "Collection Employees - La maison en bois"
   ClientHeight    =   3525
   ClientLeft      =   1140
   ClientTop       =   1515
   ClientWidth     =   4995
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   3525
   ScaleWidth      =   4995
   Begin VB.CommandButton cmdTrouble 
      Caption         =   "&Erreur"
      Height          =   315
      Left            =   3150
      TabIndex        =   8
      Top             =   2370
      Width           =   1545
   End
   Begin VB.CommandButton cmdClose 
      Caption         =   "&Fermer"
      Height          =   315
      Left            =   3150
      TabIndex        =   9
      Top             =   2820
      Width           =   1545
   End
   Begin VB.CommandButton cmdListEmployees 
      Caption         =   "&Rafra�chir la liste"
      Height          =   315
      Left            =   3150
      TabIndex        =   7
      Top             =   1950
      Width           =   1545
   End
   Begin VB.CommandButton cmdDeleteEmployee 
      Caption         =   "&Supprimer"
      Height          =   315
      Left            =   3150
      TabIndex        =   6
      Top             =   1530
      Width           =   1545
   End
   Begin VB.CommandButton cmdAddEmployee 
      Caption         =   "&Ajouter"
      Default         =   -1  'True
      Enabled         =   0   'False
      Height          =   345
      Left            =   3150
      TabIndex        =   5
      Top             =   1080
      Width           =   1545
   End
   Begin VB.ListBox lstEmployees 
      Height          =   1815
      Left            =   180
      TabIndex        =   4
      Top             =   1080
      Width           =   2715
   End
   Begin VB.TextBox txtSalary 
      Height          =   285
      Left            =   2700
      TabIndex        =   3
      Top             =   450
      Width           =   1995
   End
   Begin VB.TextBox txtName 
      Height          =   285
      Left            =   180
      TabIndex        =   1
      Top             =   450
      Width           =   2265
   End
   Begin VB.Label Label2 
      Caption         =   "Sa&laire"
      Height          =   195
      Left            =   2700
      TabIndex        =   2
      Top             =   180
      Width           =   2025
   End
   Begin VB.Label Label1 
      Caption         =   "&Nom"
      Height          =   195
      Left            =   180
      TabIndex        =   0
      Top             =   180
      Width           =   2265
   End
End
Attribute VB_Name = "frmSticks"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public sbMain As New SmallBusiness2

Private Sub cmdAddEmployee_Click()
    Dim empNew As Employee
    If Not IsNumeric(txtSalary) Then
        MsgBox "Le salaire n'est pas un montant valide."
        '   Mettre le focus sur le champ Salaire
        '   et s�lectionner tout le texte.
        With txtSalary
            .SetFocus
            .SelStart = 0
            .SelLength = Len(.Text)
        End With
        Exit Sub
    End If
    Set empNew = sbMain.AddEmployee(txtName.Text, txtSalary.Text)
    With empNew
        lstEmployees.AddItem .ID & ", " & .Name & ", " & .Salary
    End With
    With lstEmployees
        ' S�lectionne le dernier �l�ment ajout�.
        .ListIndex = .NewIndex
    End With
    txtName.Text = ""
    txtSalary.Text = ""
    txtName.SetFocus
End Sub

Private Sub cmdClose_Click()
    Unload Me
End Sub

Private Sub cmdDeleteEmployee_Click()
    Dim lngDeletedItem As Long
    With lstEmployees
        lngDeletedItem = .ListIndex
        ' V�rifie qu'un employ� a �t� s�lectionn�.
        If .ListIndex > -1 Then
            ' L'identificateur de l'employ� correspond aux
            ' six premiers caract�res de la ligne.
            sbMain.DeleteEmployee Left(lstEmployees.Text, 6)
            ' Supprime l'�l�ment s�lectionn�.
            .RemoveItem .ListIndex
            If .ListCount = 0 Then
                ' Si la liste est vide,
                '   ne fait pas de tentative pour
                '   d�finir une nouvelle s�lection.
                Exit Sub
            End If
            ' L'�l�ment effac� �tait-il en derni�re position dans
            '   la zone de liste? Si oui, son index sera
            '   sup�rieur ou �gal au nombre de la liste...
            If .ListCount <= lngDeletedItem Then
                '   ...d�finit donc la s�lection courante
                '   sur le nouvel �l�ment en fin de liste...
                .ListIndex = lngDeletedItem - 1
            Else
                '   ...sinon, conserve la s�lection dans
                '   la m�me position physique dans
                '   la liste.
                .ListIndex = lngDeletedItem
            End If
        Else
            MsgBox "Aucun employ� n'est s�lectionn�."
        End If
    End With
End Sub

Private Sub cmdListEmployees_Click()
    Dim lngCt As Long
    Dim emp As Employee
    With lstEmployees
        .Clear
        For lngCt = 1 To sbMain.EmployeeCount
            Set emp = sbMain.Employees(lngCt)
            .AddItem emp.ID & ", " & emp.Name & ", " & emp.Salary
        Next lngCt
        If .ListCount <> 0 Then
            ' Si plusieurs �l�ments existent dans la liste,
            '   s�lectionne le premier.
            .ListIndex = 0
        End If
    End With
End Sub

Private Sub cmdTrouble_Click()
    ' Alors que l'objet Collection priv�
    '   offre une protection contre les objets
    '   al�atoires ajout�s � la collection, une
    '   erreur de codage interne dans l'objet
    '   SmallBusiness2 peut encore provoquer
    '   des probl�mes.
    sbMain.Trouble
    MsgBox "Une erreur de codage interne dans l'objet SmallBusiness2 a ajout� un objet Employee non initialis� � la collection. Pour voir le probl�me, cliquez sur le bouton Rafra�chir la liste."
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' Affecte la valeur Nothing � la variable globale
    '   cach�e de cette feuille, pour lib�rer
    '   ses ressources.
    Set frmSticks = Nothing
End Sub

Private Sub txtName_Change()
    Call EnableAddButton
End Sub

Private Sub txtSalary_Change()
    Call EnableAddButton
End Sub

Private Sub txtSalary_KeyPress(KeyAscii As Integer)
    Select Case KeyAscii
        Case 48 To 57   ' Autorise les chiffres.
        Case 8          ' Autorise les retours arri�re.
        Case 46         ' Autorise les points.
        Case Else
            KeyAscii = 0
            Beep
    End Select
End Sub

Private Sub EnableAddButton()
    If (Len(txtName) > 0) And (Len(txtSalary) > 0) Then
        cmdAddEmployee.Enabled = True
    Else
        cmdAddEmployee.Enabled = False
    End If
End Sub

