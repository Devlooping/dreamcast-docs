Attribute VB_Name = "Module1"
' Constantes publiques
Public Const conMCIAppTitle = " Application pour le contr�le MCI"

' Ces constantes sont d�finies dans le fichier mmsystem.h.
Public Const conMCIErrInvalidDeviceID = 30257
Public Const conMCIErrDeviceOpen = 30263
Public Const conMCIErrCannotLoadDriver = 30266
Public Const conMCIErrUnsupportedFunction = 30274
Public Const conMCIErrInvalidFile = 30304

#If Win32 Then
    Declare Function GetFocus Lib "User32" () As Long
#Else
    Declare Function GetFocus Lib "User" () As Integer
#End If

' Variables publiques.
Public DialogCaption As String

Public Sub DisplayWarning()
    Dim WMsg As String
    Dim WTitle As String
   
    WMsg = "Avec Windows 95 ou Windows NT, Le lecteur de CD audio par d�faut d�marre automatiquement lorsque vous ins�rez un CD audio. Si cela se produit, d�sactivez le lecteur dans la barre des t�ches de Windows avant tout traitement."
    WTitle = "Avertissement"
    
    MsgBox WMsg, vbOKOnly, WTitle
End Sub

