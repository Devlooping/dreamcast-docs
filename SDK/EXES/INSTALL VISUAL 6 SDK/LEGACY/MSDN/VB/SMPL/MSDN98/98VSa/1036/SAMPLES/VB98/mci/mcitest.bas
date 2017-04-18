Attribute VB_Name = "Module2"
' Force toutes les erreurs d'ex�cution � �tre g�r�es ici.
Sub DisplayErrorMessageBox()
    Dim Msg As String
    Select Case Err
        Case conMCIErrCannotLoadDriver
            Msg = "Erreur de chargement du gestionnaire de p�riph�rique multim�dia."
        Case conMCIErrDeviceOpen
            Msg = "Le p�riph�rique n'est pas ouvert ou il est inconnu."
        Case conMCIErrInvalidDeviceID
            Msg = "Identificateur de p�riph�rique incorrect."
        Case conMCIErrInvalidFile
            Msg = "Nom de fichier incorrect."
        Case conMCIErrUnsupportedFunction
            Msg = "Le p�riph�rique ne g�re pas cette action."
        Case Else
            Msg = "Erreur inconnue (" + Str$(Err) + ")."
    End Select

    MsgBox Msg, 48, conMCIAppTIitle
End Sub

' Cette proc�dure autorise les �v�nements de Windows a �tre trait�s.
' Ceci peut �tre n�cessaire pour r�soudre les probl�mes de
' synchronisation avec les �v�nements de Windows.
' Cette proc�dure peut �tre utilis�e pour forcer une attente
' dans un traitement.
Sub WaitForEventsToFinish(NbrTimes As Integer)
    Dim i As Integer

    For i = 1 To NbrTimes
        dummy% = DoEvents()
    Next i
End Sub


