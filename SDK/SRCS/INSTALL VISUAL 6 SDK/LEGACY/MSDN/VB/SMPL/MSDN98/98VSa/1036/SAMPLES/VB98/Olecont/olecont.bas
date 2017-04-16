Attribute VB_Name = "ModOLECont"
Option Explicit

Public MDINew As Integer

Sub NewObject()
    MDINew = True
    NewOleForm
    If ParentForm.ActiveForm.OLE1.OLEType = vbOLENone Then
        Unload ParentForm.ActiveForm
    End If
End Sub

Sub DisplayInstructions()
    ' D�clare les variables locales.
    Dim MsgText
    Dim PB
    ' Initialise la variable d'arr�t du paragraphe.
    PB = Chr(10) & Chr(13) & Chr(10) & Chr(13)
    ' Affiche les instructions.
    MsgText = "Pour ins�rer un nouvel objet, cliquez sur Nouveau dans le menu Fichier, s�lectionnez ensuite un objet de la bo�te de dialogue Insertion de l'objet."
    MsgText = MsgText & PB & "Apr�s avoir enregistr� un objet ins�r� en cliquant sur Enregistrer sous, vous pouvez cliquer sur Ouvrir dans le menu Fichier pour afficher l'objet lors de sessions ult�rieures."
    MsgText = MsgText & PB & "Pour modifier un objet, double-cliquez sur l'objet pour afficher l'environnement d'�dition pour l'application de laquelle provient l'objet."
    MsgText = MsgText & PB & "Cliquez sur l'objet � l'aide du bouton droit de la souris pour afficher les attributs de l'objet."
    MsgText = MsgText & PB & "Utilisez les commandes Copier, Couper, et Collage sp�cial pour copier, couper et coller des objets."
    MsgText = MsgText & PB & "Cliquez sur Mettre � jour pour mettre � jour le sommaire de l'objet � ins�rer."
    MsgBox MsgText, 64, "Instructions de la d�monstration du contr�le conteneur OLE"
End Sub

Sub NewOleForm()
    Dim Newform As New ChildForm
    Newform.Show
    ' Affiche uniquement la bo�te de dialogue Insertion de l'objet si l'utilisateur clique sur
    ' Nouveau dans le menu Fichier.
    If MDINew Then
        ParentForm.ActiveForm.OLE1.InsertObjDlg
    End If
    
    UpdateCaption
End Sub

Sub OpenObject()
    MDINew = False
    NewOleForm
    OpenSave ("Open")
    If ParentForm.ActiveForm.OLE1.OLEType = vbOLENone Then
        Unload ParentForm.ActiveForm
    End If
End Sub

' L'ouverture d'un nouveau fichier ne marche qu'avec un fichier contenant un objet OLE Automation valide.
' Pour faire marcher cette application, suivez cette proc�dure :
' 1) Cliquez sur Nouveau dans le menu Fichier, sp�cifiez ensuite un objet.
' 2) Modifiez l'objet, puis cliquez sur Enregistrer sous du menu Fichier.
' 3) Cliquez sur la zone du contr�le menu de l'objet pour le fermer.
' 4) Cliquez sur Ouvrir dans le menu Fichier, s�lectionnez le fichier que vous venez d'enregistrer.
Sub OpenSave(Action As String)
    Dim Filenum
    Filenum = FreeFile
    ' D�finit les options et les filtres du contr�le CommonDialog.
    ParentForm.ActiveForm.CommonDialog1.Filter = _
      "Objets � ins�rer (*.OLE)|*.OLE|Tous les fichiers (*.*)|*.*"
    ParentForm.ActiveForm.CommonDialog1.FilterIndex = 1
  
    ParentForm.ActiveForm.OLE1.FileNumber = Filenum

On Error Resume Next

    Select Case Action
        Case "Save"
            ' Affiche la bo�te de dialogue Enregistrer sous.
            ParentForm.ActiveForm.CommonDialog1.ShowSave
            If Err Then
                ' Bouton Annuler a �t� s�lectionn�.
                If Err = 32755 Then
                    Exit Sub
                Else
                    MsgBox "Une erreur non g�r�e s'est produite avec la bo�te de dialogue Enregistrer sous."
                End If
            End If
            ' Ouvre et enregistre le fichier.
            Open ParentForm.ActiveForm.CommonDialog1.filename For Binary As Filenum
            If Err Then
                MsgBox (Error)
                    Exit Sub
            End If
                ParentForm.ActiveForm.OLE1.SaveToFile Filenum
            If Err Then MsgBox (Error)

        Case "Open"
            ' Affiche la bo�te de dialogue Ouvrir.
            ParentForm.ActiveForm.CommonDialog1.ShowOpen
            If Err Then
                ' Bouton Annuler a �t� s�lectionn�.
                If Err = 32755 Then
                    Exit Sub
                Else
                    MsgBox "Une erreur non g�r�e s'est produite avec la bo�te de dialogue Ouvrir."
                End If
            End If
            ' Ouvre le fichier.
            Open ParentForm.ActiveForm.CommonDialog1.filename For Binary As Filenum
            If Err Then
                Exit Sub
            End If
            ' Affiche le pointeur de souris sous forme de sablier.
            Screen.MousePointer = 11
            ParentForm.ActiveForm.OLE1.ReadFromFile Filenum
            If (Err) Then
                If Err = 30015 Then
                    MsgBox "Objet OLE invalide."
                Else
                    MsgBox Error$
                End If
                Unload ParentForm.ActiveForm
            End If
            ' Si aucune erreur ne se produit lors de l'ouverture, active l'objet.
            ParentForm.ActiveForm.OLE1.DoVerb -1

        ' D�finit les propri�t�s de la feuille maintenant que le contr�le conteneur OLE contient un objet.
        UpdateCaption
        ' R�tablit le pointeur de souris.
        Screen.MousePointer = 0
    End Select
  
    Close Filenum
End Sub

Sub UpdateCaption()
    ' D�finit les propri�t�s de la feuille maintenant que le contr�le conteneur OLE contient un objet.
    ParentForm.ActiveForm.Caption = "Objet" + ParentForm.ActiveForm.OLE1.Class 
    On Error Resume Next
End Sub

