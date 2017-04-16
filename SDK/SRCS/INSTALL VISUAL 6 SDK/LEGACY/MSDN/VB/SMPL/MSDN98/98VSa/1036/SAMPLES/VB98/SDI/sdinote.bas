Attribute VB_Name = "Module1"
'*** Module global pour l'exemple d'application Bloc-notes SDI. ***
'******************************************************************
Option Explicit

' Type d�fini par l'utilisateur pour stocker des informations au sujet des feuilles fille.
Type FormState
    Deleted As Integer
    Dirty As Integer
    Color As Long
End Type

Public FState As FormState              ' Tableau de types d�finis par l'utilisateur.
Public gFindString As String            ' Stocke le texte recherch�.
Public gFindCase As Integer             ' Cl� pour la recherche avec respect de la casse.
Public gFindDirection As Integer        ' Cl� pour la direction de la recherche.
Public gCurPos As Integer               ' Stocke la position du curseur.
Public gFirstTime As Integer            ' Cl� pour la position de d�marrage.
Public Const ThisApp = "MDINote"        ' Constante Registry App.
Public Const ThisKey = "Recent Files"   ' Constante Registry Key.


Sub EditCopyProc()
    ' Copie le texte s�lectionn� dans le Presse-papiers.
    Clipboard.SetText frmSDI.txtNote.SelText
End Sub

Sub EditCutProc()
    ' Copie le texte s�lectionn� dans le Presse-papiers.
    Clipboard.SetText frmSDI.txtNote.SelText
    ' Supprime le texte s�lectionn�.
    frmSDI.txtNote.SelText = ""
End Sub

Sub EditPasteProc()
    ' Place le texte depuis le Presse-papiers dans un contr�le actif.
    frmSDI.txtNote.SelText = Clipboard.GetText()
End Sub

Sub FileNew()
    Dim intResponse As Integer
    
    ' Si le fichier a chang�, l'enregistre.
    If FState.Dirty = True Then
        intResponse = FileSave
        If intResponse = False Then Exit Sub
    End If
    ' Efface le contenu du contr�le TextBox et met � jour le titre de la feuille.
    frmSDI.txtNote.Text = ""
    frmSDI.Caption = "Bloc-notes SDI - Sans titre"
End Sub
Function FileSave() As Integer
    Dim strFilename As String

    If frmSDI.Caption = "Bloc-notes SDI - Sans titre" Then
        ' Le fichier n'a pas encore �t� enregistr�.
        ' Obtient le nom de fichier, puis appelle la proc�dure d'enregistrement SaveFileAs.
        strFilename = GetFileName(strFilename)
    Else
        ' Le titre de la feuille contient le nom du fichier ouvert.
        strFilename = Right(frmSDI.Caption, Len(frmSDI.Caption) - 17)
    End If
    ' Appelle la proc�dure d'enregistrement. Si la variable strFilename est vide,
    ' l'utilisateur a cliqu� sur Annuler dans la bo�te de dialogue Enregistrer sous...;
    ' sinon, enregistre le fichier.
    If strFilename <> "" Then
        SaveFileAs strFilename
        FileSave = True
    Else
        FileSave = False
    End If
End Function


Sub FindIt()
    Dim intStart As Integer
    Dim intPos As Integer
    Dim strFindString As String
    Dim strSourceString As String
    Dim strMsg As String
    Dim intResponse As Integer
    Dim intOffset As Integer
    
    ' D�finit la variable offset bas�e sur la position du curseur.
    If (gCurPos = frmSDI.txtNote.SelStart) Then
        intOffset = 1
    Else
        intOffset = 0
    End If

    ' Lit la variable publique pour la position de d�marrage.
    If gFirstTime Then intOffset = 0
    ' Affecte une valeur pour la valeur de d�marrage.
    intStart = frmSDI.txtNote.SelStart + intOffset
        
    ' Sans respect de la casse, convertit la cha�ne en majuscules.
    If gFindCase Then
        strFindString = gFindString
        strSourceString = frmSDI.txtNote.Text
    Else
        strFindString = UCase(gFindString)
        strSourceString = UCase(frmSDI.txtNote.Text)
    End If
            
    ' Recherche pour la cha�ne.
    If gFindDirection = 1 Then
        intPos = InStr(intStart + 1, strSourceString, strFindString)
    Else
        For intPos = intStart - 1 To 0 Step -1
            If intPos = 0 Then Exit For
            If Mid(strSourceString, intPos, Len(strFindString)) = strFindString Then Exit For
        Next
    End If

    ' Si la cha�ne est trouv�e...
    If intPos Then
        frmSDI.txtNote.SelStart = intPos - 1
        frmSDI.txtNote.SelLength = Len(strFindString)
    Else
        strMsg = "Impossible de trouver " & Chr(34) & gFindString & Chr(34)
        intResponse = MsgBox(strMsg, 0, App.Title)
    End If
    
    ' R�initialise les variables publiques.
    gCurPos = frmSDI.txtNote.SelStart
    gFirstTime = False
End Sub

Sub GetRecentFiles()
    ' Cette proc�dure montre l'utilisation de la fonction GetAllSettings,
    ' qui retourne un tableau de valeurs de la base de registres de Windows.
    ' Dans ce cas, la base de registres contient les derniers fichiers ouverts.
    ' Utilisez l'instruction SaveSetting pour �crire le nom des fichiers les plus r�cents.
    ' Cette instruction est utilis�e dans la proc�dure WriteRecentFiles.
    Dim i As Integer
    Dim varFiles As Variant ' Variable pour stocker le tableau renvoy�.
 
    ' Obtient les fichiers les plus r�cents de la base de registres � l'aide de l'instruction GetAllSettings.
    ' ThisApp et ThisKey sont des constantes d�finies dans ce module.
    If GetSetting(ThisApp, ThisKey, "RecentFile1") = Empty Then Exit Sub
    
    varFiles = GetAllSettings(ThisApp, ThisKey)
    
    For i = 0 To UBound(varFiles, 1)
        frmSDI.mnuRecentFile(0).Visible = True
        frmSDI.mnuRecentFile(i + 1).Caption = varFiles(i, 1)
        frmSDI.mnuRecentFile(i + 1).Visible = True
    Next i
End Sub
Sub ResizeNote()
    ' Agrandit la zone de texte pour remplir la zone interne de la feuille.
    If frmSDI.picToolbar.Visible Then
        frmSDI.txtNote.Height = frmSDI.ScaleHeight - frmSDI.picToolbar.Height
        frmSDI.txtNote.Width = frmSDI.ScaleWidth
        frmSDI.txtNote.Top = frmSDI.picToolbar.Height
    Else
        frmSDI.txtNote.Height = frmSDI.ScaleHeight
        frmSDI.txtNote.Width = frmSDI.ScaleWidth
        frmSDI.txtNote.Top = 0
    End If
End Sub


Sub WriteRecentFiles(OpenFileName)
    ' Cette proc�dure utilise l'instruction SaveSettings pour �crire le nom des derniers fichiers
    ' ouverts dans la base de registres. L'instruction SaveSettings requiert
    ' trois param�tres. Deux d'entre eux sont stock�s en tant que constantes et sont
    ' d�finis dans ce module. La fonction GetAllSettings est utilis�e dans la proc�dure
    ' GetRecentFiles pour r�cup�rer les noms de fichiers stock�s dans cette proc�dure.
    
    Dim i As Integer
    Dim strFile As String
    Dim strKey As String

    ' Copie RecentFile1 dans RecentFile2, et ainsi de suite.
    For i = 3 To 1 Step -1
        strKey = "RecentFile" & i
        strFile = GetSetting(ThisApp, ThisKey, strKey)
        If strFile <> "" Then
            strKey = "RecentFile" & (i + 1)
            SaveSetting ThisApp, ThisKey, strKey, strFile
        End If
    Next i
  
    ' �crit le fichier ouvert dans le fichier le plus r�cent.
    SaveSetting ThisApp, ThisKey, "RecentFile1", OpenFileName
End Sub

