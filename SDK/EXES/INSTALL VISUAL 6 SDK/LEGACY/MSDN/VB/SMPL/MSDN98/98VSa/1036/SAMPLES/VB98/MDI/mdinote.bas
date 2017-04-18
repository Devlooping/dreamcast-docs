Attribute VB_Name = "Module1"
'***   Module global de l'application Bloc-notes MDI    ***
'**********************************************************
Option Explicit

' Type d�fini par l'utilisateur pour stocker les informations sur les feuilles filles
Type FormState
    Deleted As Integer
    Dirty As Integer
    Color As Long
End Type

Public FState()  As FormState           ' Tableau des types d�finis par l'utilisateur
Public Document() As New frmNotePad     ' Tableau des objets des feuilles filles
Public gFindString As String            ' Contient le texte � chercher.
Public gFindCase As Integer             ' Cl� pour une recherche avec casse.
Public gFindDirection As Integer        ' Cl� pour la direction de recherche.
Public gCurPos As Integer               ' Contient la location du curseur.
Public gFirstTime As Integer            ' Cl� pour position initiale.
Public gToolsHidden As Boolean          ' Contient l'�tat de la barre d'outils.
Public Const ThisApp = "MDINote"        ' Constante de l'application des registres.
Public Const ThisKey = "Recent Files"   ' Constante de la cl� des registres.


Function AnyPadsLeft() As Integer
    Dim i As Integer        ' Variable counteur

    ' Cycle � travers le tableau des documents.
    ' Retourne la valeur True s'il y a au moins un document ouvert.
    For i = 1 To UBound(Document)
        If Not FState(i).Deleted Then
            AnyPadsLeft = True
            Exit Function
        End If
    Next
End Function


Sub EditCopyProc()
    ' Copie le texte s�lectionn� dans la Presse-papiers.
    Clipboard.SetText frmMDI.ActiveForm.ActiveControl.SelText
End Sub

Sub EditCutProc()
    ' Copie le texte s�lectionn� dans la Presse-papiers.
    Clipboard.SetText frmMDI.ActiveForm.ActiveControl.SelText
    ' Supprime le texte s�lectionn�.
    frmMDI.ActiveForm.ActiveControl.SelText = ""
End Sub

Sub EditPasteProc()
    ' Place le texte du Presse-papiers dans le contr�le actif.
    frmMDI.ActiveForm.ActiveControl.SelText = Clipboard.GetText()
End Sub

Sub FileNew()
    Dim fIndex As Integer

    ' Trouve le prochain index et affiche la feuille fille.
    fIndex = FindFreeIndex()
    Document(fIndex).Tag = fIndex
    Document(fIndex).Caption = "Sans titre:" & fIndex
    Document(fIndex).Show

    ' Assurez-vous que les boutons d'�dition de la barre d'outils soient visibles.
    frmMDI.imgCutButton.Visible = True
    frmMDI.imgCopyButton.Visible = True
    frmMDI.imgPasteButton.Visible = True
End Sub

Function FindFreeIndex() As Integer
    Dim i As Integer
    Dim ArrayCount As Integer

    ArrayCount = UBound(Document)

    ' Cycle � travers le tableau des documents.
    ' Si l'un des documents vient d'�tre supprim�, alors retourne cet index.
    For i = 1 To ArrayCount
        If FState(i).Deleted Then
            FindFreeIndex = i
            FState(i).Deleted = False
            Exit Function
        End If
    Next
    ' Si aucun �l�ment dans le tableau des documents a �t� supprim�
    ' alors incr�mente de 1 les tableaux des documents et des �tats
    ' et puis retourne l'index au nouvel �l�ment.
    ReDim Preserve Document(ArrayCount + 1)
    ReDim Preserve FState(ArrayCount + 1)
    FindFreeIndex = UBound(Document)
End Function

Sub FindIt()
    Dim intStart As Integer
    Dim intPos As Integer
    Dim strFindString As String
    Dim strSourceString As String
    Dim strMsg As String
    Dim intResponse As Integer
    Dim intOffset As Integer
    
    ' D�finit la variable de d�calage bas�e sur la position du curseur.
    If (gCurPos = frmMDI.ActiveForm.ActiveControl.SelStart) Then
        intOffset = 1
    Else
        intOffset = 0
    End If

    ' Lit la position initiale de la variable publique.
    If gFirstTime Then intOffset = 0
    ' Affecte une valeur � la valeur initiale.
    intStart = frmMDI.ActiveForm.ActiveControl.SelStart + intOffset
        
    ' S'il n'est pas avec casse, convertit la cha�ne en majuscule
    If gFindCase Then
        strFindString = gFindString
        strSourceString = frmMDI.ActiveForm.ActiveControl.Text
    Else
        strFindString = UCase(gFindString)
        strSourceString = UCase(frmMDI.ActiveForm.ActiveControl.Text)
    End If
            
    ' Recherche la cha�ne.
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
        frmMDI.ActiveForm.ActiveControl.SelStart = intPos - 1
        frmMDI.ActiveForm.ActiveControl.SelLength = Len(strFindString)
    Else
        strMsg = "Impossible de trouver " & Chr(34) & gFindString & Chr(34)
        intResponse = MsgBox(strMsg, 0, App.Title)
    End If
    
    ' R�initialise les variables publiques
    gCurPos = frmMDI.ActiveForm.ActiveControl.SelStart
    gFirstTime = False
End Sub

Sub GetRecentFiles()
    ' Cette proc�dure d�montre l'utilisation de la fonction GetAllSettings,
    ' laquelle retourne un tableau de valeurs � partir de la base de registres Windows.
    ' Dans ce cas le registre contient les fichiers ouverts le plus r�cemment.
    ' Utilise la proc�dure SaveSetting pour �crire les noms des fichiers les
    ' plus r�cents. Cette proc�dure est utilis�e dans WriteRecentFiles.

    Dim i, j As Integer
    Dim varFiles As Variant ' Variable pour stocker le tableau renvoy�.
    ' Obtient les fichiers r�cents de la base de registres avec la proc�dure GetAllSettings.
    ' ThisApp et ThisKey sont des constantes d�finies dans ce module.
    If GetSetting(ThisApp, ThisKey, "RecentFile1") = Empty Then Exit Sub
    
    varFiles = GetAllSettings(ThisApp, ThisKey)
    
    For i = 0 To UBound(varFiles, 1)
        
        frmMDI.mnuRecentFile(0).Visible = True
        frmMDI.mnuRecentFile(i).Caption = varFiles(i, 1)
        frmMDI.mnuRecentFile(i).Visible = True
            ' Cycle � travers tous les documents et met � jour chaque menu.
            For j = 1 To UBound(Document)
                If Not FState(j).Deleted Then
                    Document(j).mnuRecentFile(0).Visible = True
                    Document(j).mnuRecentFile(i + 1).Caption = varFiles(i, 1)
                    Document(j).mnuRecentFile(i + 1).Visible = True
                End If
            Next j
    Next i

End Sub

Sub OptionsToolbarProc(CurrentForm As Form)
    ' Bascule la valeur de la case � cocher
    CurrentForm.mnuOptionsToolbar.Checked = Not CurrentForm.mnuOptionsToolbar.Checked
    ' S'il n'est pas une feuille MDI, d�finit la case � cocher de la feuille MDI.
    If Not TypeOf CurrentForm Is MDIForm Then
        frmMDI.mnuOptionsToolbar.Checked = CurrentForm.mnuOptionsToolbar.Checked
    End If
    ' Bascule la barre d'outils
    If CurrentForm.mnuOptionsToolbar.Checked Then
        frmMDI.picToolbar.Visible = True
    Else
        frmMDI.picToolbar.Visible = False
    End If
End Sub

Sub WriteRecentFiles(OpenFileName)
    ' Cette proc�dure utilise SaveSettings pour �crire les noms des fichiers les
    ' plus r�cents dans la base de registres syst�me. La proc�dure SaveSetting n�cessite
    ' trois param�tres. Deux de ces param�tres sont stock�s comme constantes et sont d�finis
    ' dans ce module. La fonction GetAllSettings est utilis�e dans la proc�dure GetRecentFiles
    ' pour r�cup�rer le noms des fichiers stock�s dans cette proc�dure.
    
    Dim i, j As Integer
    Dim strFile, key As String

    ' Copie RecentFile1 dans RecentFile2
    For i = 3 To 1 Step -1
        key = "RecentFile" & i
        strFile = GetSetting(ThisApp, ThisKey, key)
        If strFile <> "" Then
            key = "RecentFile" & (i + 1)
            SaveSetting ThisApp, ThisKey, key, strFile
        End If
    Next i
  
    ' �crit le nom du fichier ouvert comme �tant le fichier le plus r�cent.
    SaveSetting ThisApp, ThisKey, "RecentFile1", OpenFileName
End Sub

