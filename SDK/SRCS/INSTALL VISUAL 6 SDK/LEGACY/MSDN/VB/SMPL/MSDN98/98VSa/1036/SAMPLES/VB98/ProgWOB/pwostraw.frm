VERSION 5.00
Begin VB.Form frmStraw 
   Caption         =   "Collection Employees - La maison en paille"
   ClientHeight    =   3525
   ClientLeft      =   1140
   ClientTop       =   1515
   ClientWidth     =   4995
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   PaletteMode     =   1  'UseZOrder
   ScaleHeight     =   3525
   ScaleWidth      =   4995
   WhatsThisHelp   =   -1  'True
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
      Top             =   2790
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
      Sorted          =   -1  'True
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
Attribute VB_Name = "frmStraw"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public sbMain As New SmallBusiness1

Private Sub cmdAddEmployee_Click()
    Dim empNew As New Employee
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
    With empNew
        .ID = sbMain.NewEmployeeID
        .Name = txtName.Text
        .Salary = CDbl(txtSalary.Text)
        sbMain.Employees.Add empNew, .ID
        lstEmployees.AddItem .ID & ", " & .Name & ", " & .Salary
        With lstEmployees
            ' S�lectionne le dernier �l�ment ajout�.
            .ListIndex = .NewIndex
        End With
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
            sbMain.Employees.Remove Left(lstEmployees.Text, 6)
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
    Dim emp As Employee
    With lstEmployees
        .Clear
        For Each emp In sbMain.Employees
            .AddItem emp.ID & ", " & emp.Name & ", " & emp.Salary
            ' Cliquer sur le bouton Rafra�chir la liste, apr�s avoir
            '   appuy� sur le bouton Erreur, provoque une erreur de
            '   compatibilit� de types (soit dans l'instruction
            '   For Each, si l'�l�ment non valide est le premier
            '   de la liste, soit � l'instruction Next)
            '   lorsque Visual Basic essaie de placer la
            '   r�f�rence � frmStraw dans la variable d'it�ration
            '   emp. Pour continuer l'ex�cution, d�placez la
            '   fl�che jaune d'ex�cution sur l'instruction End Sub
            '   (ou cliquez sur End Sub et appuyez sur Ctrl+F9),
            '   puis appuyez sur F5.
        Next
        ' Si vous vous arr�tez ici, reportez-vous � la note ci-dessus.
        '
        If .ListCount <> 0 Then
            ' Si plusieurs �l�ments existent dans la liste,
            '   s�lectionne le premier.
            .ListIndex = 0
        End If
    End With
End Sub

Private Sub cmdTrouble_Click()
    ' �tant donn� que la classe Collection accepte
    '   tous les objets, une erreur de codage peut
    '   placer un objet non valide dans la collection.
    sbMain.Employees.Add Me
    MsgBox "Une r�f�rence � la feuille d'entr�e des donn�es vient d'�tre ajout�e � la collection. Pour voir le probl�me, cliquez sur le bouton Rafra�chir la liste."
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' Affectez la valeur Nothing � toutes les
    '   r�f�rences � cette feuille, pour
    '   lib�rer ses ressources.
    '   Cela demande deux op�rations:
    '   (1) Affectez la valeur Nothing � la
    '       variable globale cach�e de la feuille:
    Set frmStraw = Nothing
    '   (2) Effacez l'objet de la collection,
    '       car le bouton Erreur place une
    '       r�f�rence � la feuille dans la
    '       collection, ce qui cr�e une r�f�rence
    '       circulaire (la variable sbMain
    '       a une r�f�rence � Employees, qui
    '       a une r�f�rence � la feuille, qui
    '       a une r�f�rence � la variable
    '       sbMain) qui maintient tous les
    '       objets en vie.
    Set sbMain.Employees = Nothing
    '
    ' Bien entendu, il s'agit d'un bogue qui
    '   peut d�truire de la sorte la collection
    '   Employees de l'objet SmallBusiness;
    '   mais, apr�s tout, la Maison en paille
    '   est une solution � NE PAS appliquer.
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
