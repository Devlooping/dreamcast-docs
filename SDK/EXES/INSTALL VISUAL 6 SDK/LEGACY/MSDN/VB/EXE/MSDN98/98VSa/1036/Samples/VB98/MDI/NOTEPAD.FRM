VERSION 5.00
Begin VB.Form frmNotePad 
   Caption         =   "Sans titre"
   ClientHeight    =   3990
   ClientLeft      =   1515
   ClientTop       =   3315
   ClientWidth     =   5670
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
   MDIChild        =   -1  'True
   ScaleHeight     =   3990
   ScaleMode       =   0  'User
   ScaleWidth      =   101.07
   Begin VB.TextBox Text1 
      Height          =   3975
      HideSelection   =   0   'False
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   0
      Width           =   5655
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&Fichier"
      Begin VB.Menu mnuFileNew 
         Caption         =   "&Nouveau"
      End
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Ouvrir..."
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Fermer"
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "&Enregistrer"
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "Enregistrer &sous..."
      End
      Begin VB.Menu mnuFSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "&Quitter"
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "-"
         Index           =   0
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile1"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile2"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile3"
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile4"
         Index           =   4
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRecentFile 
         Caption         =   "RecentFile5"
         Index           =   5
         Visible         =   0   'False
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "&Edition"
      Begin VB.Menu mnuEditCut 
         Caption         =   "&Couper"
         Shortcut        =   ^X
      End
      Begin VB.Menu mnuEditCopy 
         Caption         =   "Co&pier"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuEditPaste 
         Caption         =   "C&oller"
         Shortcut        =   ^V
      End
      Begin VB.Menu mnuEditDelete 
         Caption         =   "&Supprimer"
      End
      Begin VB.Menu mnuESep1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEditSelectAll 
         Caption         =   "S�lectionner &tout"
      End
      Begin VB.Menu mnuEditTime 
         Caption         =   "&Heure/Date"
      End
   End
   Begin VB.Menu mnuSearch 
      Caption         =   "&Rechercher"
      Begin VB.Menu mnuSearchFind 
         Caption         =   "&Chercher"
      End
      Begin VB.Menu mnuSearchFindNext 
         Caption         =   "Sui&vant"
         Shortcut        =   {F3}
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsToolbar 
         Caption         =   "&Barres d'outils"
      End
      Begin VB.Menu mnuFont 
         Caption         =   "&Police"
         Begin VB.Menu mnuFontName 
            Caption         =   "FontName"
            Index           =   0
         End
      End
   End
   Begin VB.Menu mnuWindow 
      Caption         =   "&Fen�tre"
      WindowList      =   -1  'True
      Begin VB.Menu mnuWindowCascade 
         Caption         =   "&En cascade"
      End
      Begin VB.Menu mnuWindowTile 
         Caption         =   "&Mosa�que"
      End
      Begin VB.Menu mnuWindowArrange 
         Caption         =   "&R�organiser les ic�nes"
      End
   End
End
Attribute VB_Name = "frmNotePad"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'***   Feuille fille de l'application Bloc-notes MDI    ***
'**********************************************************
Option Explicit

Private Sub Form_Load()
    Dim i As Integer        ' Variable Compteur.
    
    ' Affecte le nom de la premi�re police dans l'entr�e du menu Police,
    ' puis fait une boucle sur la collection Fonts, en les ajoutant au menu.
    mnuFontName(0).Caption = Screen.Fonts(0)
    For i = 1 To Screen.FontCount - 1
        Load mnuFontName(i)
        mnuFontName(0).Caption = Screen.Fonts(i)
    Next
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    Dim strMsg As String
    Dim strFilename As String
    Dim intResponse As Integer

    ' V�rifie si le texte � �t� modifi�.
    If FState(Me.Tag).Dirty Then
        strFilename = Me.Caption
        strMsg = "Le texte de [" & strFilename & "] a �t� modifi�."
        strMsg = strMsg & vbCrLf
        strMsg = strMsg & "Enregistrer les modifications?"
        intResponse = MsgBox(strMsg, 51, frmMDI.Caption)
        Select Case intResponse
            Case 6      ' L'utilisateur a choisit Oui.
                If Left(Me.Caption, 10) = "Sans titre" Then
                    ' Le fichier n'a pas encore �t� enregistr�.
                    strFilename = "Sans titre.txt"
                    ' Obtient le nom de fichier, appelle ensuite la proc�dure d'enregistrement SaveFileAs.
                    strFilename = GetFileName(strFilename)
                Else
                   ' Le titre de la feuille contient le nom du fichier ouvert.
                    strFilename = Me.Caption
                End If
                ' Appelle la proc�dure d'enregistrement. Si la variable strFilename est vide,
                ' l'utilisateur a cliqu� sur Annuler dans la bo�te de dialogue Enregistrer sous...;
                ' sinon, enregistre le fichier.
                If strFilename <> "" Then
                    SaveFileAs strFilename
                End If
            Case 7       ' L'utilisateur a cliqu� sur Non. D�charge le fichier.
                Cancel = False
            Case 2       ' L'utilisateur a cliqu� sur Annuler. Annule le d�chargement.
                Cancel = True
        End Select
    End If
End Sub

Private Sub Form_Resize()
    ' Appelle la proc�dure de redimensionnement.
    Text1.Height = ScaleHeight
    Text1.Width = ScaleWidth
End Sub

Private Sub Form_Unload(Cancel As Integer)
    ' Montre la feuille courante comme supprim�e
    FState(Me.Tag).Deleted = True
    
    ' Masque les boutons d'�dition s'il n'y a pas une fen�tre bloc-notes.
    If Not AnyPadsLeft() Then
        frmMDI.imgCutButton.Visible = False
        frmMDI.imgCopyButton.Visible = False
        frmMDI.imgPasteButton.Visible = False
        ' Bascule la variable publique d'�tat de l'outil
        gToolsHidden = True
        ' Appelle la proc�dure qui donne la liste des fichiers les plus r�cent
        GetRecentFiles
    End If
End Sub

Private Sub mnuEditCopy_Click()
    ' Appelle la proc�dure Copier
    EditCopyProc
End Sub

Private Sub mnuEditCut_Click()
    ' Appelle la proc�dure Couper
    EditCutProc
End Sub

Private Sub mnuEditDelete_Click()
    ' Si le pointeur de la souris n'est pas � la fin du bloc-notes...
    If Screen.ActiveControl.SelStart <> Len(Screen.ActiveControl.Text) Then
        ' Si rien n'est s�lectionn�, �tend la s�lection d'une unit�.
        If Screen.ActiveControl.SelLength = 0 Then
            Screen.ActiveControl.SelLength = 1
            ' Si le pointeur de la souris est sur une ligne vide, �tend la s�lection de deux unit�s.
            If Asc(Screen.ActiveControl.SelText) = 13 Then
                Screen.ActiveControl.SelLength = 2
            End If
        End If
        ' Supprime le texte s�lectionn�.
        Screen.ActiveControl.SelText = ""
    End If
End Sub

Private Sub mnuEditPaste_Click()
    ' Appelle la proc�dure Coller.
    EditPasteProc
End Sub

Private Sub mnuEditSelectAll_Click()
    ' Utilise les propri�t�s SelStart et SelLength pour s�lectionner le texte.
    frmMDI.ActiveForm.Text1.SelStart = 0
    frmMDI.ActiveForm.Text1.SelLength = Len(frmMDI.ActiveForm.Text1.Text)
End Sub

Private Sub mnuEditTime_Click()
    ' Ins�re la date et l'heure.
    Text1.SelText = Now
End Sub

Private Sub mnuFileClose_Click()
    ' D�charge cette feuille.
    Unload Me
End Sub

Private Sub mnuFileExit_Click()
    ' Le d�chargement de la feuille MDI appelle l'�v�nement QueryUnload
    ' pour chaque feuille fille, puis la feuille MDI.
    ' Si l'argument Cancel est d�finit � True dans un
    ' �v�nement QueryUnload, annule le d�chargement.
    Unload frmMDI
End Sub

Private Sub mnuFileNew_Click()
    ' Appelle la proc�dure pour la cr�ation d'une nouvelle feuille
    FileNew
End Sub

Private Sub mnuFontName_Click(index As Integer)
    ' Affecte la police s�lectionn�e � la propri�t� FontName de la zone d'�dition
    Text1.FontName = mnuFontName(index).Caption
End Sub

Private Sub mnuFileOpen_Click()
    ' Appelle la proc�dure Ouvrir un nouveau fichier
    FileOpenProc
End Sub

Private Sub mnuFileSave_Click()
    Dim strFilename As String

    If Left(Me.Caption, 10) = "Sans titre" Then
        ' Le fichier n'a pas encore �t� enregistr�.
        ' Lit le nom du fichier puis appelle la proc�dure d'enregistrement (GetFileName).
        strFilename = GetFileName(strFilename)
    Else
        ' Le titre de la feuille contient le nom du fichier ouvert.
        strFilename = Me.Caption
    End If
    ' Appelle la proc�dure d'enregistrement. Si Filename = Empty alors
    ' l'utilisateur a choisi Annuler dans la bo�te de dialogue Enregistrer sous;
    ' sinon enregistre le fichier.
    If strFilename <> "" Then
        SaveFileAs strFilename
    End If
End Sub

Private Sub mnuFileSaveAs_Click()
    Dim strSaveFileName As String
    Dim strDefaultName As String
    
    ' Affecte le titre de la feuille � la variable.
    strDefaultName = Me.Caption
    If Left(Me.Caption, 10) = "Sans titre" Then
        ' Le fichier n'a pas encore �t� enregistr�.
        ' Lit le nom de fichier puis appelle la proc�dure d'enregistrement, strSaveFileName.
        strSaveFileName = GetFileName("Sans titre.txt")
        If strSaveFileName <> "" Then SaveFileAs (strSaveFileName)
        ' Met � jour la liste des fichiers r�cenment ouverts dans le tableau du contr�le menu fichier.
        UpdateFileMenu (strSaveFileName)
    Else
        ' Le titre de la feuille contient le nom du fichier ouvert.
        strSaveFileName = GetFileName(strDefaultName)
        If strSaveFileName <> "" Then SaveFileAs (strSaveFileName)
        ' Met � jour la liste des fichiers r�cenment ouverts dans le groupe de contr�les du menu fichier.
        UpdateFileMenu (strSaveFileName)
    End If

End Sub

Private Sub mnuOptions_Click()
    ' D�finit la propri�t� Checked avec la valeur de la propri�t� Visible.
    mnuOptionsToolbar.Checked = frmMDI.picToolbar.Visible
End Sub

Private Sub mnuOptionsToolbar_Click()
    ' Appelle la proc�dure de barre d'outils en passant en r�f�rence
    ' l'instance de la feuille.
    OptionsToolbarProc Me
End Sub

Private Sub mnuRecentFile_Click(index As Integer)
    ' Appelle la proc�dure d'ouverture de fichiers avec une r�f�rence
    ' au nom du fichier s�lectionn�
    OpenFile (mnuRecentFile(index).Caption)
    ' Met � jour la liste des fichiers r�cenment ouverts dans le groupe de contr�les du menu fichier.
    GetRecentFiles
End Sub

Private Sub mnuSearchFind_Click()
    ' Si la zone d'�dition contient du texte, l'affecte
    ' � la zone d'�dition de la feuille Chercher, sinon
    ' affecte la derni�re valeur de findtext.
    If Me.Text1.SelText <> "" Then
        frmFind.Text1.Text = Me.Text1.SelText
    Else
        frmFind.Text1.Text = gFindString
    End If
    ' D�finit la variable publique pour commencer au d�but.
    gFirstTime = True
    ' D�finit la case � cocher �gale � la variable publique
    If (gFindCase) Then
        frmFind.chkCase = 1
    End If
    ' Affiche la feuille Chercher.
    frmFind.Show vbModal
End Sub

Private Sub mnuSearchFindNext_Click()
    ' Si la variable publique n'est pas vide, appelle la proc�dure
    ' find, sinon appelle le menu Chercher
    If Len(gFindString) > 0 Then
        FindIt
    Else
        mnuSearchFind_Click
    End If
End Sub

Private Sub mnuWindowArrange_Click()
    ' R�organise les ic�nes pour toutes les feuilles r�duites.
    frmMDI.Arrange vbArrangeIcons
End Sub

Private Sub mnuWindowCascade_Click()
    ' Met en cascade toutes les feuilles filles.
    frmMDI.Arrange vbCascade
End Sub

Private Sub mnuWindowTile_Click()
    ' R�partit horizontalement toutes les feuilles filles
    frmMDI.Arrange vbTileHorizontal
End Sub

Private Sub Text1_Change()
    ' D�finit la variable publique pour montrer que le texte a chang�.
    FState(Me.Tag).Dirty = True
End Sub


