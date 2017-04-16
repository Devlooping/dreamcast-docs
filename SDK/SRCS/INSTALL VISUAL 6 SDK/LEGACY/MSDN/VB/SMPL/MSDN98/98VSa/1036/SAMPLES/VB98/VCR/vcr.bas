Attribute VB_Name = "VCRModule"
'********************************************************
' Sujet: Module g�n�ral pour l'exemple d'application VCR. 
'        Contient des proc�dures partag�es.
'********************************************************
Option Explicit

' Effectue l'instanciation de la classe Recorder.
Public Recorder As New clsRecorder

'**********************************************
' Sujet:    Active ou d�sactive les boutons de la
'           feuille VCR bas�e sur le mode en cours.
' Entr�es:  Bouton: la bouton de commande appelant
'           la proc�dure.
'**********************************************
Sub ButtonManager(Button As Control)
    Dim vntControl As Variant   ' La valeur du contr�le
    
    ' D�termine sur quel bouton de fonction l'utilisateur
    ' a appuy� et met � jour les boutons et la classe Recorder.
    Select Case Button
        Case frmVCR.cmdPlay, frmVCR.cmdForward, frmVCR.cmdRewind
            frmVCR.cmdPause.Enabled = True
            frmVCR.cmdStop.Enabled = True
            frmVCR.cmdRec.Enabled = False
            frmVCR.cmdDown.Enabled = False
            frmVCR.cmdUp.Enabled = False
            Recorder.Enabled = False
        Case frmVCR.cmdRec
            frmVCR.cmdPause.Enabled = True
            frmVCR.cmdStop.Enabled = True
            frmVCR.cmdPlay.Enabled = False
            frmVCR.cmdForward.Enabled = False
            frmVCR.cmdRewind.Enabled = False
            frmVCR.cmdDown.Enabled = False
            frmVCR.cmdUp.Enabled = False
            Recorder.Enabled = False
        Case frmVCR.cmdPause
            frmVCR.cmdPause.Enabled = False
        Case frmVCR.cmdStop
            frmVCR.cmdStop.Enabled = False
            frmVCR.cmdPause.Enabled = False
            frmVCR.cmdRec.Enabled = True
            frmVCR.cmdPlay.Enabled = True
            frmVCR.cmdForward.Enabled = True
            frmVCR.cmdRewind.Enabled = True
            frmVCR.cmdDown.Enabled = True
            frmVCR.cmdUp.Enabled = True
            Recorder.Enabled = True
    End Select
    ' affecte le bouton � son contr�le Shape correspondant.
    vntControl = Button.Name
    vntControl = Right$(vntControl, Len(vntControl) - 3)
    vntControl = "shp" & vntControl
    ' appelle la routine pour mettre � jour les indicateurs.
    HighlightButton vntControl
End Sub
'*********************************************************
' Sujet:    Active ou d�sactive les contr�les de forme 
'           sur la feuille VCR bas�e sur le mode en cours.
' Entr�es:  MyControl: le contr�le Shape pass�
'           � la proc�dure.
'*********************************************************
Sub HighlightButton(MyControl As Variant)
    Dim i As Integer    ' Variable Compteur 
    
    ' Parcours la collection Controls.
    For i = 0 To frmVCR.Controls.Count - 1
        ' Test pour un contr�le Shape.
        If TypeOf frmVCR.Controls(i) Is Shape Then
            ' S'il s'agit du contr�le pass�, le rend
            ' visible, sinon le rend invisible.
            If frmVCR.Controls(i).Name = MyControl Then
                frmVCR.Controls(i).Visible = True
            Else
                frmVCR.Controls(i).Visible = False
            End If
        End If
    Next
End Sub
'******************************************************
' Sujet:    Stocke le dernier num�ro de cha�ne pour
'           le restaurer apr�s l'enregistrement.
' Entr�es:  Channel: le num�ro de cha�ne pass�
'           � la proc�dure.
' Renvois:  La cha�ne stock�e dans la variable Static.
'******************************************************
Function SaveChannel(Channel As Variant) As Integer
    Static intChannel As Integer ' Stocke la cha�ne.
    
    ' Affecte la valeur � la variable Static.
    If Channel <> 0 Then
        intChannel = Channel
    End If
    
    ' Renvoie le num�ro de cha�ne.
    SaveChannel = intChannel
End Function
