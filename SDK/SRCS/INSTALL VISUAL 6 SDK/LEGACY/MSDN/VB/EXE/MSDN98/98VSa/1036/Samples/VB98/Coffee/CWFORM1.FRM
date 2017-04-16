VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "CoffeeWatch2"
   ClientHeight    =   3765
   ClientLeft      =   1800
   ClientTop       =   1500
   ClientWidth     =   5160
   LinkTopic       =   "Form1"
   ScaleHeight     =   3765
   ScaleWidth      =   5160
   Begin VB.CommandButton cmdMT 
      Caption         =   "Exemple de traitement &multithread"
      Height          =   375
      Left            =   840
      TabIndex        =   4
      Top             =   3240
      Width           =   3255
   End
   Begin VB.ListBox lstCallBacks 
      Height          =   2205
      Left            =   2640
      TabIndex        =   3
      Top             =   840
      Width           =   2175
   End
   Begin VB.CommandButton cmdCallBacks 
      Caption         =   "D�marre la r�ception des &rappels"
      Height          =   615
      Left            =   2640
      TabIndex        =   1
      Top             =   120
      Width           =   2175
   End
   Begin VB.CommandButton cmdEvents 
      Caption         =   "D�marre la r�ception des �v�nements Coffee&Ready"
      Height          =   615
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2175
   End
   Begin VB.ListBox lstEvents 
      Height          =   2205
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   2175
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' Stockage de niveau module pour l'objet
'   CoffeeMonitor partag�. La variable est
'   d�clar�e en tant que WithEvents: les
'   �v�nements CoffeeMonitor peuvent ainsi
'   �tre g�r�s.
Private WithEvents mwcmnEvents As CoffeeMonitor
Attribute mwcmnEvents.VB_VarHelpID = -1
' Objet Connector utilis� pour obtenir
'   une r�f�rence � l'objet CoffeeMonitor
'   partag�.
Private mcctEvents As Connector

' Stockage de niveau module pour une r�f�rence
'   � l'objet CoffeeMonitor2 utilis� dans
'   l'exemple de la m�thode de rappel.
Private mcm2CallBacks As CoffeeMonitor2
' Stockage pour une r�f�rence � l'objet de rappel.
Private mNotifyMe As New NotifyMe

' Bouton de commande pour lancer et arr�ter la
'   r�ception des �v�nements.
Private Sub cmdEvents_Click()
    Static blnInUse As Boolean
    
    If blnInUse Then
        ' Affecter la valeur Nothing � la variable
        '   WithEvents pour d�connecter l'objet
        '   de ses proc�dures d'�v�nement.
        '   Ainsi, les �v�nements ne seront plus re�us.
        Set mwcmnEvents = Nothing
        '
        ' Lib�re de l'objet Connector.
        Set mcctEvents = Nothing
        cmdEvents.Caption = "D�marre la r�ception des �v�nements Coffee&Ready"
    Else
        Set mcctEvents = New Connector
        '
        ' L'objet Connector fournit une r�f�rence
        '   � l'objet CoffeeMonitor partag�. Lorsque la
        '   r�f�rence est plac�e dans la variable
        '   WithEvents, l'objet est connect� � sa
        '   proc�dure d'�v�nement. Ainsi ,l'objet
        '   peut commencer � recevoir des �v�nements.
        Set mwcmnEvents = mcctEvents.CoffeeMonitor
        cmdEvents.Caption = "ARR�TE la r�ception des �v�nements CoffeeReady"
    End If
    blnInUse = True Xor blnInUse
End Sub

' Bouton de commande pour lancer et arr�ter
'   les rappels.
Private Sub cmdCallBacks_Click()
    Static blnInUse As Boolean
    Dim ct2 As New Connector2
    
    If blnInUse Then
        ' Informe l'objet CoffeeMonitor2 que les
        '   rappels (Callbacks) ne sont plus n�cessaires.
        Call mcm2CallBacks.CeaseCallBacks(mNotifyMe)
        '
        ' Lib�re la copie partag�e de l'objet CoffeeMonitor2.
        Set mcm2CallBacks = Nothing
        cmdCallBacks.Caption = "D�marre la r�ception des rappels"
    Else
        ' Obtient un objet Connector2 et l'utilise pour
        '   obtenir une r�f�rence � la copie partag�e
        '   de l'objet CoffeeMonitor2.
        Set ct2 = New Connector2
        Set mcm2CallBacks = ct2.CoffeeMonitor2
        '
        ' Informe l'objet CoffeeMonitor2 de commencer
        '   � ex�cuter des appels � l'objet NotifyMe
        '   (l'objet est cr�� ici implicitement puisque
        '   la variable est d�clar�e comme As New).
        Call mcm2CallBacks.TellMeReady(mNotifyMe)
        cmdCallBacks.Caption = "ARR�TE la r�ception des rappels"
    End If
    blnInUse = True Xor blnInUse
End Sub

Private Sub cmdMT_Click()
    ' Le lancement de l'exemple de traitement
    '   multithread n�cessite la d�sactivation
    '   des rappels et des �v�nements.
    '   Demandez � l'utilisateur si cela peut �tre
    '   fait. Si la r�ponse est vbNo, continuez et
    '   d�marrez l'exemple de traitement multithread
    '   quand m�me.
    If (Not mwcmnEvents Is Nothing) Or _
            (Not mcm2CallBacks Is Nothing) Then
        Select Case MsgBox("Les notifications d'�venements et de rappels devraient �tre arr�t�es avant de lancer l'exemple de thread exigeant des activit�s importantes du processeur. Les arr�ter maintenant?", _
                vbYesNoCancel, "D�marre l'exemple de traitement multithread")
            Case vbYes
                ' Ceci ne d�charge pas la feuille;
                '   les exemples sont simplement arr�t�s.
                Call Form_Unload(False)
            Case vbCancel
                Exit Sub
        End Select
    End If
    cmdMT.Enabled = False
    frmThread.Show vbModeless
End Sub

' Ferme tous les exemples en cours d'ex�cution.
'
Private Sub Form_Unload(Cancel As Integer)
    If Not mwcmnEvents Is Nothing Then
        Call cmdEvents_Click
    End If
    If Not mcm2CallBacks Is Nothing Then
        Call cmdCallBacks_Click
    End If
End Sub

' Lorsque l'objet CoffeeMonitor envoie un
'   �v�nement CoffeeReady, l'ajoute � la zone
'   de liste. Si celle-ci contient plus de dix
'   �l�ments, le plus ancien est effac�.
'
Private Sub mwcmnEvents_CoffeeReady()
    With lstEvents
        .AddItem Format$(Now, "ddd hh:mm:ss"), 0
        If .ListCount > 10 Then .RemoveItem 10
    End With
End Sub

