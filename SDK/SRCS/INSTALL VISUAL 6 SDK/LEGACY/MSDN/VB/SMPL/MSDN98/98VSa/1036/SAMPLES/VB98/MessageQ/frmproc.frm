VERSION 5.00
Begin VB.Form frmProcessClaim 
   Caption         =   "Form1"
   ClientHeight    =   3435
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   5415
   LinkTopic       =   "Form1"
   ScaleHeight     =   3435
   ScaleWidth      =   5415
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdSubmitClaim 
      Caption         =   "&Soumettre r�clamation"
      Height          =   255
      Left            =   2880
      TabIndex        =   9
      Top             =   3000
      Visible         =   0   'False
      Width           =   2055
   End
   Begin VB.CommandButton cmdNewClaim 
      Caption         =   "&Nouvelle r�clamation"
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   3000
      Visible         =   0   'False
      Width           =   2055
   End
   Begin VB.CommandButton cmdApprove 
      Caption         =   "&Approve"
      Height          =   255
      Left            =   2880
      TabIndex        =   16
      Top             =   3000
      Width           =   2055
   End
   Begin VB.TextBox txtName 
      BackColor       =   &H8000000F&
      Height          =   285
      Left            =   2040
      TabIndex        =   0
      Text            =   "John Doe"
      Top             =   120
      Width           =   3135
   End
   Begin VB.TextBox txtStreet 
      BackColor       =   &H8000000F&
      Height          =   285
      Left            =   2040
      TabIndex        =   1
      Text            =   "1600 Pennsilvania Ave"
      Top             =   600
      Width           =   3135
   End
   Begin VB.TextBox txtCity 
      BackColor       =   &H8000000F&
      Height          =   285
      Left            =   2040
      TabIndex        =   2
      Text            =   "Washington"
      Top             =   1080
      Width           =   3135
   End
   Begin VB.TextBox txtState 
      BackColor       =   &H8000000F&
      Height          =   285
      Left            =   2040
      TabIndex        =   3
      Text            =   "D.C."
      Top             =   1560
      Width           =   3135
   End
   Begin VB.OptionButton optAuto 
      Caption         =   "V�hicule"
      Enabled         =   0   'False
      Height          =   255
      Left            =   1800
      TabIndex        =   4
      Top             =   2040
      Value           =   -1  'True
      Width           =   975
   End
   Begin VB.OptionButton optHome 
      Caption         =   "Maison"
      Enabled         =   0   'False
      Height          =   255
      Left            =   3000
      TabIndex        =   5
      Top             =   2040
      Width           =   975
   End
   Begin VB.OptionButton optBoat 
      Caption         =   "Bateau"
      Enabled         =   0   'False
      Height          =   255
      Left            =   4200
      TabIndex        =   6
      Top             =   2040
      Width           =   975
   End
   Begin VB.TextBox txtAmountOfClaim 
      BackColor       =   &H8000000F&
      Height          =   285
      Left            =   2040
      TabIndex        =   8
      Text            =   "2000"
      Top             =   2520
      Width           =   3135
   End
   Begin VB.Label Label1 
      Caption         =   "Nom:"
      Height          =   255
      Left            =   120
      TabIndex        =   15
      Top             =   120
      Width           =   1335
   End
   Begin VB.Label lblStreet 
      Caption         =   "Rue:"
      Height          =   255
      Left            =   120
      TabIndex        =   14
      Top             =   600
      Width           =   1335
   End
   Begin VB.Label lblCity 
      Caption         =   "Ville:"
      Height          =   255
      Left            =   120
      TabIndex        =   13
      Top             =   1080
      Width           =   1335
   End
   Begin VB.Label lblState 
      Caption         =   "�tat:"
      Height          =   255
      Left            =   120
      TabIndex        =   12
      Top             =   1560
      Width           =   1335
   End
   Begin VB.Label lblClaim 
      Caption         =   "R�clamation:"
      Height          =   255
      Left            =   120
      TabIndex        =   11
      Top             =   2040
      Width           =   1335
   End
   Begin VB.Label lblClaimAmmount 
      Caption         =   "Montant de la r�clamation:"
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   2520
      Width           =   1935
   End
End
Attribute VB_Name = "frmProcessClaim"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
' --------------------------------------------------------
' La feuille Template pour le processus UI du d�roulement des op�rations
' Ce feuille Template est utilis�e par diff�rentes applications :
'  CLAIM_ENTRY       : App qui entre la r�clamation (d�bute le d�roulement des op�rations)
'  CLAIM_PROCESSING  : App qui effectue le processus de r�clamation (d�place le d�roulement des op�rations de l'une des files vers une autre file)
'  ACCOUNTING_PROCESSING  : App qui finit le d�roulement des op�rations (proc�de au d�roulement dans la file finale)
'
'  Il est bon de savoir que la logique du noyau au cours de chacune de ces �tapes est la m�me :
'  a) Obtention des informations
'  b) Processus des informations
'  c) D�placement des informations
'
'  [11/13/97, IvoSa] Cr��e
' --------------------------------------------------------

#If CLAIM_PROCESSING Then
    Const FORM_CAPTION = "Processus de r�clamation d'assurance"
#ElseIf ACCOUNTING_PROCESSING Then
    Const FORM_CAPTION = "Processus de r�clamation de comptabilit�"
#ElseIf CLAIM_ENTRY Then
    Const FORM_CAPTION = "Entr�e de r�clamation"
#End If

Private m_mqSendQueue As MSMQQueue 'File pour le service des r�clamations

' Couleurs souhait�es
Const WINDOW_BACKGROUND = &H80000005
Const BUTTON_FACE = &H8000000F
' --------------------------------------------
' Configure la feuille avec les donn�es de r�clamation
'  [in] objStrBag      : Objet qui contient les donn�es
'  [in] objMQNextQueue : File d'emplacement des �l�ments approuv�s
' --------------------------------------------
Public Sub Initailzie(ByVal objStrBag As StringBag, ByVal objMQNextQueue As MSMQQueue)
    Set m_mqSendQueue = objMQNextQueue
    
    txtName = objStrBag.Retrieve(KEY_NAME)

' -------------------------------------------------
' Les messages dans la file de comptabilit� ne prennent pas en charge rue, ville, �tat, etc...
' -------------------------------------------------
#If CLAIM_PROCESSING Then
    txtStreet = objStrBag.Retrieve(KEY_STREET)
    txtCity = objStrBag.Retrieve(KEY_CITY)
    txtState = objStrBag.Retrieve(KEY_STATE)
        
    Dim strClaim As String
    strClaim = objStrBag.Retrieve(KEY_CLAIM)
    
    If (strClaim = CLAIM_AUTO) Then
        optAuto.Value = True
    ElseIf (strClaim = CLAIM_HOME) Then
        optHome.Value = True
    Else
        optBoat.Value = True
    End If
#End If
    txtAmountOfClaim = CStr(objStrBag.Retrieve(KEY_AMOUNT))
End Sub

Private Sub cmdApprove_Click()
    ApproveClaim
End Sub

' ------------------------------------------------
' La r�clamation a �t� approuv�e, envoyer vers la file suivante
'  [11/13/97, IvoSa]
' ------------------------------------------------
Sub ApproveClaim()
    ' Si aucune file suivante, alors quitter
    If (m_mqSendQueue Is Nothing) Then
        MsgBox "D�roulement des op�rations fini"
        Unload Me
        Exit Sub
    End If

    ' R�unit les propri�t�s bag avec le d�roulement des op�rations & l'envoyer
    Dim objStringBag As StringBag
    Set objStringBag = New StringBag
    objStringBag.Add KEY_NAME, txtName
    objStringBag.Add KEY_AMOUNT, txtAmountOfClaim

    objStringBag.serilizeToQueue m_mqSendQueue, txtName

    MsgBox "Envoie du message fini"
    Unload Me
End Sub

Private Sub cmdNewClaim_Click()
    ClearClaimInfo
End Sub

' ----------------------------------------------
' Soumet une r�clamation
' ----------------------------------------------
Private Sub cmdSubmitClaim_Click()
    SubmitClaim
    MsgBox "R�clamation soumise avec succ�s !"
End Sub

' ------------------------------------------------
' Configure l'UI de la feuille
' ------------------------------------------------
Private Sub Form_Load()
    Me.Caption = FORM_CAPTION
#If CLAIM_PROCESSING Then
    ' Couleur
    txtName.BackColor = BUTTON_FACE
    txtStreet.BackColor = BUTTON_FACE
    txtCity.BackColor = BUTTON_FACE
    txtState.BackColor = BUTTON_FACE
    txtAmountOfClaim.BackColor = BUTTON_FACE
    
#ElseIf ACCOUNTING_PROCESSING Then
    ' Visibilit�
    lblStreet.Visible = False
    lblCity.Visible = False
    lblState.Visible = False
    lblClaim.Visible = False
    
    optAuto.Visible = False
    optBoat.Visible = False
    optHome.Visible = False

    txtStreet.Visible = False
    txtCity.Visible = False
    txtState.Visible = False
    
    ' Couleur
    txtAmountOfClaim.BackColor = BUTTON_FACE
#ElseIf CLAIM_ENTRY Then
    ' Visible
    cmdApprove.Visible = False
    cmdNewClaim.Visible = True
    cmdSubmitClaim.Visible = True
    
    ' Couleur de fond
    txtName.BackColor = WINDOW_BACKGROUND
    txtStreet.BackColor = WINDOW_BACKGROUND
    txtCity.BackColor = WINDOW_BACKGROUND
    txtState.BackColor = WINDOW_BACKGROUND
    txtAmountOfClaim.BackColor = WINDOW_BACKGROUND
    
    ' Verrouillage
    txtName.Locked = False
    txtStreet.Locked = False
    txtCity.Locked = False
    txtState.Locked = False
    txtAmountOfClaim.Locked = False
    
    ' Activ�e
    optAuto.Enabled = True
    optBoat.Enabled = True
    optHome.Enabled = True

    ' Ouvre une file de soumissions...
    OpenMessageQueueForSubmit
#End If

End Sub

' -----------------------------
' Efface les informations de r�clamation
' -----------------------------
Sub ClearClaimInfo()
    txtName = ""
    txtStreet = ""
    txtCity = ""
    txtState = ""
    
    optAuto.Value = True
    txtAmountOfClaim = ""
    
End Sub

' ----------------------------------------------
' Soumet une r�clamation
' ----------------------------------------------
Private Sub SubmitClaim()

' Remplit une propri�t� bag avec les valeurs � envoyer...
Dim objStrBag As StringBag
    Set objStrBag = New StringBag
    objStrBag.Add KEY_NAME, Trim(txtName)
    objStrBag.Add KEY_STREET, Trim(txtStreet)
    objStrBag.Add KEY_CITY, Trim(txtCity)
    objStrBag.Add KEY_STATE, Trim(txtState)
    
    If (optAuto.Value) Then
        objStrBag.Add KEY_CLAIM, CLAIM_AUTO
    ElseIf (optHome.Value) Then
        objStrBag.Add KEY_CLAIM, CLAIM_HOME
    Else
        objStrBag.Add KEY_CLAIM, CLAIM_BOAT
    End If
    
    ' Prend en charge en tant que valeur monn�taire
    objStrBag.Add KEY_AMOUNT, CCur(txtAmountOfClaim)
    
    ' --------------------------------------------------
    ' GARDE LA COLLECTION DE DONN�ES DANS UN MESSAGE DE LA FILE
    ' --------------------------------------------------
    objStrBag.serilizeToQueue m_mqSendQueue, objStrBag.Retrieve(KEY_NAME)
End Sub


' ---------------------------------------------------------
' Ouvre la file du message 'R�clamations' pour soumettre les messages
'  [11/13/97, IvoSa] Cr��e
'  [12/6/97, IvoSa] V�rification des erreurs ajout�e
' ---------------------------------------------------------
Sub OpenMessageQueueForSubmit()
Dim lngError As Long
Dim strError As String

    On Error Resume Next
        Set m_mqSendQueue = openMessageQueue(INSURANCE_CLAIMS_QUEUE_NAME, MQ_SEND_ACCESS, True)
        lngError = Err
        strError = Err.Description
    On Error GoTo 0

    If lngError Then
        MsgBox ERROR_TEXT_RUNADMINTOOL & "(texte d'erreur sp�cifique lu: " & strError & ")"
        
        Stop ' Permet � l'app d'�tre d�bogu�e maintenant...
        End ' Continuation de l'ex�cution non recommand�e � ce stade
    End If
End Sub

Private Sub lblClaimAmmount_Click()

End Sub

Private Sub txtCity_Change()

End Sub

Private Sub txtName_Change()

End Sub
