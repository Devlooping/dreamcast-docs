Attribute VB_Name = "modQueueSupportHelper"
Option Explicit
' --------------------------------------------------
'  Contient les fonctions HELPER utiles pour l'acc�s
'  aux files de messages.  Ces fonctions ne sont PAS
'  con�ues pour �tre appel�es � partir d'applications
'  (elles ne font rien).  Elles sont con�ues pour
'  supporter les fonctions principales
'
'  [10/11/97, IvoSa] Cr��e
' --------------------------------------------------

Const LEN_GUID_WITH_BRACKETS = 38
Const LEN_GUID_WITHOUT_BRACKETS = LEN_GUID_WITH_BRACKETS - 2

' ------------------------------------------------------------------
' Appel�e lorsque le GUID de file correct est introuvable dans le
' Registre syst�me (absent ou incorrect)
'  [in] strQueueName : Nom de la file � parcourir
'  [retval]          : GUID de la file QUEUE
'
'  [10/11/97, IvoSa] Cr��
' ------------------------------------------------------------------
Function LookupGUIDAndPersistToRegisty(ByVal strQueueName As String) As String
Dim strQueueGUID As String
        
    ' Rechercher la file
    strQueueGUID = getGUIDOfMessageQueue(strQueueName)

    ' Contr�le de validit� sur le GUID retourn�
    If (Not (IsGUID(strQueueGUID))) Then
        Err.Raise qeBOGUS_GUID, "LookupGUIDAndPersistToRegisty", "GUID ID MSMQ non valide: " & qeBOGUS_GUID
    End If

    ' Inscrire le GUID avec le nom de la file
    SaveSetting regAPPLICATION_NAME, strQueueName, regKEY_NAME, strQueueGUID
    
    ' GUID de la file...
    LookupGUIDAndPersistToRegisty = strQueueGUID
End Function

' --------------------------------------------------
' V�rifie la validit� d'une cha�ne 'guid'
'  [in] strQueueGUID  : Nom de la file � ouvrir
'
'  [10/11/97, IvoSa] Cr��
' --------------------------------------------------
Function IsGUID(ByVal strQueueGUID As String) As Boolean
    strQueueGUID = Trim(strQueueGUID)
    
    Dim lngStrLen As Long
    lngStrLen = Len(strQueueGUID)
    
    Select Case lngStrLen
    Case LEN_GUID_WITH_BRACKETS
    Case LEN_GUID_WITHOUT_BRACKETS
    Case Else
        IsGUID = False: Exit Function
    End Select

    ' On peut souhaiter des tests compl�mentaires
    IsGUID = True
End Function


' --------------------------------------------------
' Affiche un avertissement � l'utilisateur de l'application
'  [10/11/97, IvoSa] Cr��
' --------------------------------------------------
Sub Queue_Warning(strWarning As String)
#If Not SUPPRESS_WARNINGS Then
    MsgBox strWarning, , "Avertissement sur acc�s � la file"
#End If
End Sub

' -----------------------------------------------------------
' V�rifie qu'un GUID ne contient pas de {}'
'  [in] strGuid : GUID avec ou sans {}'s
'
'  [11/12/97, IvoSa] Cr��
' -----------------------------------------------------------
Function getGUIDWithoutBrackets(strGuid As String) As String
    Dim lngStrLen As Long
    lngStrLen = Len(strGuid)
    
    Select Case lngStrLen
    Case LEN_GUID_WITH_BRACKETS
        getGUIDWithoutBrackets = Mid(strGuid, 2, LEN_GUID_WITHOUT_BRACKETS)
    Case LEN_GUID_WITHOUT_BRACKETS
        getGUIDWithoutBrackets = strGuid
    Case Else
        Err.Raise qeBOGUS_GUID, "getGUIDWithoutBrackets", strGuid & " GUID incorrect, longueur 0."
    End Select
End Function

' -----------------------------------------------------------
' V�rifie qu'un GUID ne contient pas de {}'
'  [in] strGuid : GUID w/ or w/o {}'s
'
'  [11/12/97, IvoSa] Cr��e
' -----------------------------------------------------------
Function getGUIDWithBrackets(strGuid As String) As String
    Dim lngStrLen As Long
    lngStrLen = Len(strGuid)
    
    Select Case lngStrLen
    Case LEN_GUID_WITH_BRACKETS
        getGUIDWithBrackets = strGuid
    Case LEN_GUID_WITHOUT_BRACKETS
        getGUIDWithBrackets = "{" & strGuid & "}"
    Case Else
        Err.Raise qeBOGUS_GUID, "getGUIDWithBrackets", strGuid & " GUID incorrect, longueur 0."
    End Select
End Function


' --------------------------------------------------
' Informations de file n�cessaires pour cr�er une file
'
'  [in] strPath  : chemin de la file
'  [in] strLabel  : �tiquette de la file
'  [retval] QueueInfo n�cessaire pour cr�er l'objet
'
'  NOTES: L'objet de cette fonction consiste �
'         combiner la cr�ation/initialisation de cet
'         objet en une unit� atomique.

'
'  [11/12/97, IvoSa] Cr��e
' --------------------------------------------------
Function getQueueInfoForCreation(strPath As String, strLabel As String) As MSMQQueueInfo
 Dim objQInfo As MSMQQueueInfo

    ' A FAIRE : ajouter du code pour v�rifier si une file de ce nom existe d�j�.
    
    Set objQInfo = New MSMQQueueInfo
    objQInfo.PathName = strPath
    objQInfo.Label = strLabel
    
    Set getQueueInfoForCreation = objQInfo
End Function


' --------------------------------------------------
' Informations de file n�cessaires pour ouvrir une file par nom de GUID
'
'  [in] strGUIDForQueue : GUID de la file
'  [retval] QueueInfo n�cessaire pour cr�er l'objet
'
'  NOTES: L'objet de cette fonction consiste �
'         combiner la cr�ation/initialisation
'         de cet objet en une unit� atomique.
'
'  [11/12/97, IvoSa] Cr��e
' --------------------------------------------------
Function getQueueInfoForLogon(strGUIDForQueue As String) As MSMQQueueInfo
    Dim objQI As MSMQQueueInfo
    Set objQI = New MSMQQueueInfo
    objQI.FormatName = "PUBLIC=" & getGUIDWithoutBrackets(strGUIDForQueue)
    Set getQueueInfoForLogon = objQI
End Function

' --------------------------------------------------
' Informations de file n�cessaires pour supprimer un file par �tiquette
'
'  [in] strLabelForQueue  : �tiquette de la file
'  [retval] QueueInfo n�cessaire pour supprimer l'objet
'
'  NOTES: L'objet de cette fonction consiste �
'         combiner la cr�ation/initialisation
'         de cet objet en une unit� atomique.
'
'  [11/12/97, IvoSa] Cr��e
' --------------------------------------------------
Function getQueueInfoForDeletion(strLabelForQueue As String) As MSMQQueueInfo
    Dim objQuery As MSMQQuery
    Set objQuery = New MSMQQuery
    
    Dim objQI As MSMQQueueInfo
    Dim objQIs As MSMQQueueInfos
    Set objQIs = objQuery.LookupQueue(Label:=strLabelForQueue)
    
    objQIs.Reset
    
    ' A FAIRE : on peut v�rifier s'il y en a plusieurs
    Set getQueueInfoForDeletion = objQIs.Next
End Function


' --------------------------------------------------
' Retourne le GUID qui identifie une file de messages
'  [in] strQueueName : Nom de la file � ouvrir
'
'  SUPPOSITION: Nous sommes branch�s sur le r�seau
'  lorsque ceci est appel� (sinon on risque le blocage)
'
'  [10/11/97, IvoSa] Cr��e
' --------------------------------------------------
Function getGUIDOfMessageQueue(ByVal strQueueName As String) As String
Dim objQuery As MSMQQuery
Dim objQInfos As MSMQQueueInfos
Dim objQInfo As MSMQQueueInfo
Dim objQinfoTemp As MSMQQueueInfo
    
    ' Nouvelle requ�te pour rechercher les files de messages
    Set objQuery = New MSMQQuery
    
    ' Ex�cute la requ�te, retourne un ensemble de requ�tes
    Set objQInfos = objQuery.LookupQueue(Label:=strQueueName)
    
    ' Retourne au d�but (est-ce n�cessaire ?)
    objQInfos.Reset

    ' Parcourt les files de messages et recherche les notres
    Dim lngCount As Long
    Set objQinfoTemp = objQInfos.Next
    While Not (objQinfoTemp Is Nothing)
        Set objQInfo = objQinfoTemp
        lngCount = lngCount + 1
        
        ' Avance jusqu'� l'�l�ment suivant
        Set objQinfoTemp = objQInfos.Next
    Wend
    
    ' Une seule file permise avec l'ID sp�cifi�
    If (lngCount > 1) Then
        Queue_Warning "La file compte plus d'un (" & CStr(lngCount) & ").  Plusieurs files ont la m�me �tiquette, donc: 1) la requ�te de file doit �tre plus sp�cifique ou 2) toutes les files portant cette �tiquette sauf une doivent �tre supprim�es.  Utilisation par d�faut de la premi�re file trouv�e."
    End If
    
    getGUIDOfMessageQueue = objQInfo.QueueGuid
End Function


