Attribute VB_Name = "Module1"
Option Explicit

Declare Function timeGetTime Lib "winmm.dll" () As Long

' CoffeeWatch doit �tre un Exe ActiveX, car
'   il expose l'objet public NotifyMe
'   utilis� pour l'exemple de la m�thode de
'   rappel. Il ne sera jamais ex�cut� comme
'   un composant mais comme une application
'   autonome.
Sub Main()
    If App.StartMode = vbSModeStandalone Then
        Form1.Show
    End If
End Sub
