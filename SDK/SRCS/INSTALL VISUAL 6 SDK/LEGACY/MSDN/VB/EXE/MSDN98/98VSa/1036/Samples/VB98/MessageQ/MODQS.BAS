Attribute VB_Name = "modQueueSupport"
Option Explicit
' --------------------------------------------------
'  Contient des fonctions utiles pour les files de
'  messages.  Ces fonctions sont pr�vues pour �tre
'  appel�es � partir des applications pour cr�er,
'  d�truire, g�rer et acc�der les files.
'
'  [11/10/97, IvoSa] Cr�ation
' --------------------------------------------------

' Informations du Registre pour g�rer les ID de file, etc.
Global Const regAPPLICATION_NAME = "MessageQueueAdministration"
Global Const regKEY_NAME = "QueueGUID"
Global Const regNOT_FOUND = "<Not Found>"

' Heure
Global Const ONE_SECOND = 1000 '1000ms
Global Const FIVE_SECONDS = ONE_SECOND * 5


' Heures g�n�r�es �ventuellement par ce module
Public Enum QueueErrors
    qeNO_QUEUE = 30000
    qeBOGUS_GUID = 30001
End Enum

' --------------------------------------------------
' Ouvre une file de messages
'  [in] strQueueName : Nom de la file � ouvrir
'  [in] qaAccessMode : Mode d'acc�s (lecture/�criture)
'  [in] boolPromotUser : Demande � l'utilisateur si besoin d'autres informations ?
'  [out] objQueueInfo : Informations de file (utiles pour l'appelant)
'  [retval] : Objet file
'
' Cette fonction g�n�rale permet d'ouvrir une file
' de messages pour l'utilisateur, que l'utilisateur (ou la file)
' soit ou ne soit pas actuellement connect� au r�seau.
' Cette fonction doit �tre appel�e au moins une fois
' avec une connexion r�seau sinon elle �chouera.
' (Lors du premier appel de cette fonction sur une
' machine/utilisateur notera le GUID des files
' de messages pour future r�f�rence)
'
' [11/10/97, IvoSa] Cr�ation
' --------------------------------------------------
Public Function openMessageQueue(ByVal strQueueName As String, ByVal qaAccessMode As MQACCESS, Optional ByVal boolPromotUser As Boolean = True, Optional objQueueInfo As MSMQQueueInfo) As MSMQQueue
    Dim strQueueGUID As String
    
    ' -------------------------------------------------------------------
    ' D�termine le GUID de la file de message
    ' -------------------------------------------------------------------
    
    ' Lit le GUID qui identifie de fa�on unique la file
    strQueueGUID = GetSetting(regAPPLICATION_NAME, strQueueName, regKEY_NAME, regNOT_FOUND)
    
    ' S'il n'y a pas de valeur sp�cifi�e pour la file
    If (strQueueGUID = regNOT_FOUND) Then
        
        ' Essaye de demander � l'utilisateur
        If (boolPromotUser = True) Then
            Dim lngYesNo As VbMsgBoxResult
            lngYesNo = MsgBox("Il n'y a pas de file inscrite localement pour '" & strQueueName & ". Voulez-vous rechercher cette file sur le r�seau ('Non' pour abandonner)?", vbYesNo)
            
            ' S'il r�pond non, abandonner
            If (lngYesNo = vbNo) Then
                Err.Raise qeNO_QUEUE, "OpenMessageQueue", "Il n'y a pas de file inscrite pour cet utilisateur/machine pour " & strQueueName
            End If
        End If
        
        strQueueGUID = LookupGUIDAndPersistToRegisty(strQueueName)
    End If
    
    ' -------------------------------------------------------------------
    ' Essaye d'ouvrir la file de messages...
    ' -------------------------------------------------------------------
    Dim lngError As Long
    Dim strError As Long
    On Error Resume Next
        Set objQueueInfo = getQueueInfoForLogon(strQueueGUID)
        Set openMessageQueue = objQueueInfo.Open(qaAccessMode, MQ_DENY_NONE)
        lngError = Err.Number
        strError = Err.Description
    On Error GoTo 0

    ' -------------------------------------------------
    ' G�re les erreurs �ventuelles...
    ' -------------------------------------------------
    Select Case lngError
    Case 0  ' Pas d'erreur
        Exit Function ' C'est fini
    
    Case MQ_ERROR_QUEUE_NOT_FOUND ' La file est mal inscrite (ou plus probablement d�truite et recr��e)
        
        ' Essaye de demander � l'utilisateur
        If (boolPromotUser = True) Then
            lngYesNo = MsgBox("Les informations locales pour '" & strQueueName & " sont incorrects. Voulez-vous rechercher cette file sur le r�seau ('Non' pour abandonner)?", vbYesNo)
            
            ' S'il r�pond non, abandonner
            If (lngYesNo = vbNo) Then
                Err.Raise qeNO_QUEUE, "OpenMessageQueue", "Il n'y a pas de file inscrite pour cet utilisateur/machine pour " & strQueueName
            End If
        End If
        
        ' --------------------------------------------------
        ' Essaye d'ouvrir la file
        ' --------------------------------------------------
        ' Si une erreur se produit de nouveau, remonter-la simplement
        strQueueGUID = LookupGUIDAndPersistToRegisty(strQueueName)
        Set objQueueInfo = getQueueInfoForLogon(strQueueGUID)
        Set openMessageQueue = objQueueInfo.Open(qaAccessMode, MQ_DENY_NONE)
    
    Case Else ' Pas une erreur g�r�e, la remonter.
        Err.Raise lngError, , strError
    End Select
    
End Function



' --------------------------------------------------
' Create message queue
'  [in] strQueueName     : Nom de la file � ouvrir
'  [in] strQueueLocation : Emplacement de la file ('MACHINE\CHEMIN')
'  [in] boolPromptUser   : Demande � l'utilisateur en cas de probl�me de cr�ation de la file
'  [in] boolWorldReadable: Rend la file visible par tous ou uniquement par son cr�ateur...
'
'  [retval] T/F : �tait une 'nouvelle' file cr��e
'
'  Cette fonction g�n�rale permet de cr�er une nouvelle file de messages
'
'  [11/10/97, IvoSa] Cr�ation
'  [12/6/97, IvoSa]  File rendue 'Lisible par tous'
' --------------------------------------------------
Public Function createMessageQueue(ByVal strQueueName As String, ByVal strQueueLocation As String, Optional ByVal boolPromptUser As Boolean = True, Optional ByVal boolWorldReadable As Boolean = True) As Boolean
Dim objQInfo As MSMQQueueInfo
Dim boolNewQueueCreated As Boolean
    boolNewQueueCreated = True ' commence � vrai
    
    ' A FAIRE : ajouter du code pour v�rifier si une file de ce nom existe d�j�.
    
    ' Tentative de cr�ation de la file
    Dim lngError As Long
    Dim strError As Long
    On Error Resume Next
        getQueueInfoForCreation(strQueueLocation, strQueueName).Create
        lngError = Err.Number
        strError = Err.Description
    On Error GoTo 0

    Dim lngYesNo As Long
    ' -------------------------------------------------
    ' G�re les erreurs �ventuelles...
    ' -------------------------------------------------
    Select Case lngError
    Case 0  ' Pas d'erreur
        GoTo exit_function ' [11/19,97, IvoSa] C'est fini
    
    Case MQ_ERROR_QUEUE_EXISTS 'La file est mal inscrite (ou plus probablement d�truite et recr��e)
                
        ' Essaye de demander � l'utilisateur
        If (boolPromptUser = True) Then
            lngYesNo = MsgBox("La file '" & strQueueName & "' existe.  Voulez-vous la cr�er et en cr�er une nouvelle? ", vbYesNo)
            
            ' S'il r�pond non, abandonner
            If (lngYesNo = vbNo) Then
                ' La file existe et ils sont satisfaits, quitter.
                boolNewQueueCreated = False
                GoTo exit_function
            End If
        Else
            ' Par s�curit�, g�n�re une erreur
            Err.Raise MQ_ERROR_QUEUE_EXISTS, "CreateMessageQueue", "La file existe '" & strQueueName & "'"
        End If
        
        ' --------------------------------------------------
        ' Tentative de suppression de la file
        ' --------------------------------------------------
        getQueueInfoForDeletion(strQueueName).Delete
        
        ' --------------------------------------------------
        ' Tentative de cr�ation d'une nouvelle file
        ' --------------------------------------------------
        getQueueInfoForCreation(strQueueLocation, strQueueName).Create IsWorldReadable:=boolWorldReadable ' [12/6/97, IvoSa] Acc�s public aux files...
        
        ' Si une erreur se produit de nouveau, remonter-la simplement
    
    Case Else ' Pas une erreur g�r�e, la remonter.
        Err.Raise lngError, , strError
    End Select

exit_function:
    createMessageQueue = boolNewQueueCreated
End Function

' --------------------------------------------------
' Supprime une file de messages
'  [in] strQueueName : Nom de la file � ouvrir
'  [in] boolPromotUser : Demande � l'utilisateur si besoin d'autres informations ?
'
'  Cette fonction g�n�rale permet de supprimer une file de messages pour l'utilisateur.'
'  [11/12/97, IvoSa] Cr�ation
' --------------------------------------------------
Public Sub deleteMessageQueue(ByVal strQueueName As String, Optional ByVal boolPromotUser As Boolean = True)
    Dim strQueueGUID As String
    
    ' -------------------------------------------------------------------
    ' D�termine le GUID de la file de message
    ' -------------------------------------------------------------------
    
    ' Lit le GUID qui identifie de fa�on unique la file
    strQueueGUID = GetSetting(regAPPLICATION_NAME, strQueueName, regKEY_NAME, regNOT_FOUND)
    
    ' S'il n'y a pas de valeur sp�cifi�e pour la file ?
    If (strQueueGUID = regNOT_FOUND) Then
        
        ' Essaye de demander � l'utilisateur
        If (boolPromotUser = True) Then
            Dim lngYesNo As VbMsgBoxResult
            lngYesNo = MsgBox("Il n'y a pas de file inscrite localement pour '" & strQueueName & ". Voulez-vous rechercher cette file sur le r�seau ('Non' pour abandonner)?", vbYesNo)
            
            ' S'il r�pond non, abandonner
            If (lngYesNo = vbNo) Then
                Err.Raise qeNO_QUEUE, "deleteMessageQueue", "Il n'y a pas de file inscrite pour cet utilisateur/machine pour " & strQueueName
            End If
        End If
        
        strQueueGUID = LookupGUIDAndPersistToRegisty(strQueueName)
    End If
    
    ' -------------------------------------------------------------------
    ' Tentative de suppression de la file de messages...
    ' -------------------------------------------------------------------
    getQueueInfoForLogon(strQueueGUID).Delete
End Sub


' --------------------------------------------------
'  Retourne les �tiquettes des messages de la file
'  [in] objQueue     : File de messages
'
'  [11/10/97, IvoSa] Cr�ation
' --------------------------------------------------
Public Function getLabelsOfMessagesInQueue(ByVal objQueue As MSMQQueue) As Collection
Dim objMessage As MSMQMessage
Dim colLables As Collection
    
    ' Collection de stockage des �tiquettes
    Set colLables = New Collection
    
    ' Atteind le premier �l�ment de la file
    objQueue.Reset
    
    ' Parcourt tous les �l�ments et ajoute les �tiquettes � la collection
    Set objMessage = objQueue.PeekCurrent(, , ONE_SECOND) ' ONE_SECOND n�cessaire (le d�lai par d�faut est infini)
    While Not (objMessage Is Nothing)
        colLables.Add objMessage.Label
        Set objMessage = objQueue.PeekNext(, , ONE_SECOND) ' ONE_SECOND n�cessaire (le d�lai par d�faut est infini)
    Wend
    
    ' Retourne la collection d'�tiquettes de messages
    Set getLabelsOfMessagesInQueue = colLables
End Function


' -----------------------------------------------------
' � partir de l'�tiquette de message, on extraie le message
' [in] objQueue     : File de messages
' [in] strLabel     : �tiquette souhait�e
'
' [11/10/97, IvoSa] Cr�ation
' -----------------------------------------------------
Public Function getMessageGivenLabelAndQueue(ByVal objQueue As MSMQQueue, ByVal strLabel As String) As MSMQMessage
Dim objMessage As MSMQMessage
    
    objQueue.Reset ' Atteind le premier �l�ment
        
    ' Parcourt tous les �l�ments et ajoute les �tiquettes � la collection
    Set objMessage = objQueue.PeekCurrent(, , ONE_SECOND) ' ONE_SECOND n�cessaire (le d�lai par d�faut est infini)
    While Not (objMessage Is Nothing)
        If objMessage.Label = strLabel Then GoTo found_label
        Set objMessage = objQueue.PeekNext(, , ONE_SECOND) ' ONE_SECOND n�cessaire (le d�lai par d�faut est infini)
    Wend
        
    Set getMessageGivenLabelAndQueue = Nothing
    Exit Function

found_label:

    Set getMessageGivenLabelAndQueue = objQueue.ReceiveCurrent(receivetimeout:=FIVE_SECONDS)
End Function


' -----------------------------------------------------------
' Envoie un message dans une file
' [in] objQueue
' [in] strMessageLabel
' [in] strMessage
' [in] mdMode:
'       MQMSG_DELIVERY_RECOVERABLE = Enregistre sur le disque (survit au red�marrage)
'       MQMSG_DELIVERY_EXPRESS     = Garde en m�moire (ne survit pas au red�marrage)
' [11/12/97, IvoSa] Cr�ation
' -----------------------------------------------------------
Sub sendMessageToQueue(ByVal objQueue As MSMQQueue, strMessageLabel As String, strMessage As String, Optional ByVal mdMode As MQMSGDELIVERY = MQMSG_DELIVERY_RECOVERABLE)
    Dim objMsg As MSMQMessage
    
    Set objMsg = New MSMQMessage
    objMsg.Label = strMessageLabel
    objMsg.Body = strMessage
    objMsg.Delivery = mdMode
    
    objMsg.Send objQueue
End Sub

' -----------------------------------------------------------
' Lit les �tiquettes des messages de la file et d�finit un
'  �v�nement d'attente pour les nouveaux messages
' [in] objQueue : File de messages
' [out] colMsgLabels : �tiquettes des messages de la file
' [11/13/97, IvoSa] Cr�ation
' -----------------------------------------------------------
Public Function getEventFireOnNextMessageArrival(ByVal objQueue As MSMQQueue, Optional colMsgLabels As Collection) As MSMQEvent

    ' -------------------------------------------------
    ' Lit les �tiquettes des messages existants et
    '  avance le curseur jusqu'� la fin de la file
    ' -------------------------------------------------
    Set colMsgLabels = getLabelsOfMessagesInQueue(objQueue)
    
    Dim objQEvent As MSMQEvent
    Set objQEvent = New MSMQEvent
    
    objQueue.EnableNotification objQEvent, MQMSG_CURRENT
    Set getEventFireOnNextMessageArrival = objQEvent
End Function

