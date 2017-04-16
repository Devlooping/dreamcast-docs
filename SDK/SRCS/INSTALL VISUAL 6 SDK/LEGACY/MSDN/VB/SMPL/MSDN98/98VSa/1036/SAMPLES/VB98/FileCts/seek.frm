VERSION 5.00
Begin VB.Form WinSeek 
   BackColor       =   &H00C0C0C0&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Recherche de fichier"
   ClientHeight    =   3495
   ClientLeft      =   1920
   ClientTop       =   1890
   ClientWidth     =   4050
   BeginProperty Font 
      Name            =   "MS Sans Serif"
      Size            =   8.25
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H00000080&
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3495
   ScaleWidth      =   4050
   Begin VB.PictureBox Picture2 
      BorderStyle     =   0  'None
      Height          =   2895
      Left            =   120
      ScaleHeight     =   2895
      ScaleWidth      =   3855
      TabIndex        =   8
      Top             =   600
      Visible         =   0   'False
      Width           =   3855
      Begin VB.ListBox lstFoundFiles 
         Height          =   2205
         Left            =   120
         TabIndex        =   11
         Top             =   420
         Width           =   3375
      End
      Begin VB.Label lblCount 
         Caption         =   "0"
         Height          =   255
         Left            =   1560
         TabIndex        =   10
         Top             =   120
         Width           =   1095
      End
      Begin VB.Label lblfound 
         Caption         =   "&Fichiers trouv�s:"
         Height          =   255
         Left            =   120
         TabIndex        =   9
         Top             =   120
         Width           =   1695
      End
   End
   Begin VB.PictureBox Picture1 
      BorderStyle     =   0  'None
      Height          =   2715
      Left            =   0
      ScaleHeight     =   2715
      ScaleWidth      =   3735
      TabIndex        =   2
      Top             =   120
      Width           =   3735
      Begin VB.DriveListBox drvList 
         Height          =   1530
         Left            =   2040
         TabIndex        =   7
         Top             =   480
         Width           =   1575
      End
      Begin VB.DirListBox dirList 
         Height          =   1440
         Left            =   2040
         TabIndex        =   6
         Top             =   900
         Width           =   1575
      End
      Begin VB.FileListBox filList 
         Height          =   2040
         Left            =   120
         TabIndex        =   5
         Top             =   480
         Width           =   1785
      End
      Begin VB.TextBox txtSearchSpec 
         Height          =   285
         Left            =   2040
         TabIndex        =   4
         Text            =   "*.*"
         Top             =   120
         Width           =   1575
      End
      Begin VB.Label lblCriteria 
         Caption         =   "&Crit�re de recherche:"
         Height          =   255
         Left            =   120
         TabIndex        =   3
         Top             =   120
         Width           =   1815
      End
   End
   Begin VB.CommandButton cmdSearch 
      BackColor       =   &H00C0C0C0&
      Caption         =   "&Rechercher"
      Default         =   -1  'True
      Height          =   450
      Left            =   270
      TabIndex        =   0
      Top             =   2880
      Width           =   1200
   End
   Begin VB.CommandButton cmdExit 
      BackColor       =   &H00C0C0C0&
      Caption         =   "&Quitter"
      Height          =   420
      Left            =   2130
      TabIndex        =   1
      Top             =   2910
      Width           =   1200
   End
End
Attribute VB_Name = "WinSeek"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim SearchFlag As Integer   ' Utilis� comme indicateur pour annuler, etc.

Private Sub cmdExit_Click()
    If cmdExit.Caption = "&Quitter" Then
        End
    Else                    ' Si l'utilisateur clique sur Annuler, arr�te la recherche.
        SearchFlag = False
    End If
End Sub

Private Sub cmdSearch_Click()
' Initialise la recherche et effectue une recherche r�cursive.
Dim FirstPath As String, DirCount As Integer, NumFiles As Integer
Dim result As Integer
  ' V�rifie la derni�re action de l'utilisateur.
    If cmdSearch.Caption = "&R�initialiser" Then  ' S'il ne s'agit que d'une r�initialisation,
        ResetSearch
        txtSearchSpec.SetFocus
        Exit Sub
    End If

    ' Met � jour dirList.Path si ce dossier est diff�rent du
    ' dossier actuellement s�lectionn�, sinon effectue la recherche.
    If dirList.Path <> dirList.List(dirList.ListIndex) Then
        dirList.Path = dirList.List(dirList.ListIndex)
        Exit Sub         ' Quitte de fa�on � ce que l'utilisateur puisse consulter la liste
                        ' avant d'effectuer la recherche.
    End If

    ' Poursuit la recherche.
    Picture2.Move 0, 0
    Picture1.Visible = False
    Picture2.Visible = True

    cmdExit.Caption = "Annuler"

    filList.Pattern = txtSearchSpec.Text
    FirstPath = dirList.Path
    DirCount = dirList.ListCount

    ' Commence la recherche r�cursive de dossiers.
    NumFiles = 0                       ' R�initialise l'indicateur des fichiers trouv�s.
    result = DirDiver(FirstPath, DirCount, "")
    filList.Path = dirList.Path
    cmdSearch.Caption = "&R�initialiser"
    cmdSearch.SetFocus
    cmdExit.Caption = "&Quitter"
End Sub

Private Function DirDiver(NewPath As String, DirCount As Integer, BackUp As String) As Integer
' Recherche les dossiers de fa�on r�cursive � partir de NewPath...
' NewPath est recherch� de cette fa�on.
' BackUp est � l'origine de cette r�cursivit�.
' DirCount est le nombre de sous-dossiers contenus dans ce dossier.
Static FirstErr As Integer
Dim DirsToPeek As Integer, AbandonSearch As Integer, ind As Integer
Dim OldPath As String, ThePath As String, entry As String
Dim retval As Integer
    SearchFlag = True           ' D�finit l'indicateur de sorte que l'utilisateur puisse arr�ter.
    DirDiver = False            ' Affecte la valeur True s'il y a une erreur.
    retval = DoEvents()         ' V�rifie les �v�nements (par exemple si l'utilisateur clique sur Annuler).
    If SearchFlag = False Then
        DirDiver = True
        Exit Function
    End If
    On Local Error GoTo DirDriverHandler
    DirsToPeek = dirList.ListCount                  ' Combien de dossier sous celui-ci?
    Do While DirsToPeek > 0 And SearchFlag = True
        ' Enregistre l'ancien chemin d'acc�s pour la prochaine r�cursivit�.
        OldPath = dirList.Path
        dirList.Path = NewPath
        If dirList.ListCount > 0 Then
            ' Acc�de au bas du n�ud.
            dirList.Path = dirList.List(DirsToPeek - 1)
            AbandonSearch = DirDiver((dirList.Path), DirCount%, OldPath)
        End If
        ' Monte d'un niveau dans les dossiers
        DirsToPeek = DirsToPeek - 1
        If AbandonSearch = True Then Exit Function
    Loop
    ' Appelle la fonction pour �num�rer les fichiers.
    If filList.ListCount Then
        If Len(dirList.Path) <= 3 Then              ' V�rifie pour 2 octets/caract�res.
            ThePath = dirList.Path                  ' Si vous �tes � la racine, ne changez pas de niveau...
        Else
            ThePath = dirList.Path + "\"            ' Sinon met une barre oblique inverse "\" avant le nom du fichier.

        End If
        For ind = 0 To filList.ListCount - 1        ' Ajoute des fichiers conformes de ce dossier � la zone de liste.
            entry = ThePath + filList.List(ind)
            lstFoundFiles.AddItem entry
            lblCount.Caption = Str(Val(lblCount.Caption) + 1)
        Next ind
    End If
    If BackUp <> "" Then        ' S'il existe un dossier sup�rieur, d�placement dans celui-ci.
        dirList.Path = BackUp
    End If
    Exit Function
DirDriverHandler:
    If Err = 7 Then             ' Si une erreur M�moire �puis�e se produit, la zone de liste est satur�e.
        DirDiver = True         ' Cr�e Msg$ et affecte la valeur de retour AbandonSearch.
        MsgBox " La zone de liste est pleine. Recherche abandonn�e..."
        Exit Function           ' Notez que la proc�dure EXIT r�initialise ERR � 0.
    Else                        ' Sinon, affiche un message d'erreur et quitte.
        MsgBox Error
        End
    End If
End Function

Private Sub DirList_Change()
    ' Met � jour la zone de liste des fichiers, en synchronisant la zone de liste des dossiers.
    filList.Path = dirList.Path
End Sub

Private Sub DirList_LostFocus()
    dirList.Path = dirList.List(dirList.ListIndex)
End Sub

Private Sub DrvList_Change()
    On Error GoTo DriveHandler
    dirList.Path = drvList.Drive
    Exit Sub

DriveHandler:
    drvList.Drive = dirList.Path
    Exit Sub
End Sub

Private Sub Form_Load()
    Picture2.Move 0, 0
    Picture2.Width = WinSeek.ScaleWidth
    Picture2.BackColor = WinSeek.BackColor
    lblCount.BackColor = WinSeek.BackColor
    lblCriteria.BackColor = WinSeek.BackColor
    lblfound.BackColor = WinSeek.BackColor
    Picture1.Move 0, 0
    Picture1.Width = WinSeek.ScaleWidth
    Picture1.BackColor = WinSeek.BackColor
End Sub

Private Sub Form_Unload(Cancel As Integer)
    End
End Sub

Private Sub ResetSearch()
    ' R�initialise avant d'effectuer une nouvelle recherche.
    lstFoundFiles.Clear
    lblCount.Caption = 0
    SearchFlag = False                  ' Indicateur indiquant que la recherche est en cours.
    Picture2.Visible = False
    cmdSearch.Caption = "&Rechercher"
    cmdExit.Caption = "&Quitter"
    Picture1.Visible = True
    dirList.Path = CurDir: drvList.Drive = dirList.Path ' R�initialise le chemin d'acc�s.
End Sub

Private Sub txtSearchSpec_Change()
    ' Met � jour la zone de liste des fichiers si l'utilisateur change de mod�le.
    filList.Pattern = txtSearchSpec.Text
End Sub

Private Sub txtSearchSpec_GotFocus()
    txtSearchSpec.SelStart = 0          ' Met en surbrillance la s�lection saisie.
    txtSearchSpec.SelLength = Len(txtSearchSpec.Text)
End Sub

