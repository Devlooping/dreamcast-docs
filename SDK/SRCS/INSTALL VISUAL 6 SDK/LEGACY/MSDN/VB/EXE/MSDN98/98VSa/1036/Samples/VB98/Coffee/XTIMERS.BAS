Attribute VB_Name = "XTimerSupport"
Option Explicit
'================================================
' AVERTISSEMENT! Vous ne DEVEZ PAS cliquer sur
'   le bouton Fin pendant le d�bogage de ce
'   projet! En mode Arr�t, n'effectuez pas de
'   modifications: cela r�initialise le projet!
'
' Ce module est dangereux car il utilise l'API
'   SetTimer et l'op�rateur AddressOf pour
'   initialiser une minuterie en code seul. Lorsque
'   ce type de minuterie est d�fini, le syst�me
'   continue d'appeler la fonction TimerProc
'   M�ME LORSQUE VOUS REPASSEZ EN MODE CR�ATION.
'
' La fonction TimerProc n'�tant pas disponible
'   en mode Cr�ation, le syst�me provoquera
'   une ERREUR DE PROGRAMME dans Visual Basic.
'
' Lors du d�bogage de ce module, v�rifiez que
'   toutes les minuteries du syst�me ont �t�
'   arr�t�es (en utilisant la fonction KillTimer)
'   avant de repasser en mode Cr�ation. La v�rification
'   peut �tre faite en appelant la proc�dure SCRUB
'   depuis la fen�tre Ex�cution.
'
' Les minuteries utilisant des fonctions Callback sont
'   implicitement dangereuses. Il est plus s�r d'utiliser 
'   des contr�les Timer pendant une grande partie de la
'   proc�dure de d�veloppement, et de repasser � des fonctions
'   callback tout � la fin.
'==================================================

' �quivaut � augmenter la taille du tableau maxti
'   lorsque plus de minuteries sont n�cessaires.
Const MAXTIMERINCREMEMT = 5

Private Type XTIMERINFO
    xt As XTimer
    id As Long
    blnReentered As Boolean
End Type

Declare Function SetTimer Lib "user32" (ByVal hWnd As Long, ByVal nIDEvent As Long, ByVal uElapse As Long, ByVal lpTimerProc As Long) As Long
Declare Function KillTimer Lib "user32" (ByVal hWnd As Long, ByVal nIDEvent As Long) As Long

' maxti est un tableau d'objets XTimer actifs. Il est
'   pr�f�rable d'utiliser un tableau de types d�finis par
'   l'utilisateur au lieu d'un objet Collection. Cela
'   permet d'obtenir une liaison pr�coce lors du d�clenchement
'   de l'�v�nement Tick de l'objet XTimer.
Private maxti() As XTIMERINFO
'
' La variable mintMaxTimers donne la dimension du tableau maxti
'   � un moment donn�.
Private mintMaxTimers As Integer

' La fonction BeginTimer est appel�e par un objet XTimer
' -------------------   lorsque la propri�t� Interval de
'   l'objet XTimer a une valeur diff�rente de z�ro.
'
' La fonction utilise les appels API requis pour initialiser
'   une minuterie.
'   Si la cr�ation de la minuterie r�ussit, la fonction place
'   une r�f�rence � l'objet XTimer dans le tableau maxti. Cette
'   r�f�rence sera utilis�e pour appeler la m�thode qui
'   d�clenche l'�v�nement Tick de l'objet XTimer.
'
Public Function BeginTimer(ByVal xt As XTimer, ByVal Interval As Long)
    Dim lngTimerID As Long
    Dim intTimerNumber As Integer
    
    lngTimerID = SetTimer(0, 0, Interval, AddressOf TimerProc)
    ' Si la fonction SetTimer r�ussit, une valeur diff�rente de z�ro
    '   est renvoy�e. Si une minuterie ne peut
    '   �tre obtenue, d�clenche une erreur.
    If lngTimerID = 0 Then Err.Raise vbObjectError + 31013, , "Aucune minuterie de disponible"
    
    ' La boucle suivante localise le premier emplacement vide
    '   dans le tableau maxti. Si la limite sup�rieure
    '   est atteinte, une erreur se produit et le tableau est
    '   agrandi. Si vous compilez cette DLL en code natif,
    '   ne d�sactivez PAS la v�rification des limites!
    For intTimerNumber = 1 To mintMaxTimers
        If maxti(intTimerNumber).id = 0 Then Exit For
    Next
    '
    ' Si aucun espace libre n'est trouv�, augmente
    '   la taille du tableau.
    If intTimerNumber > mintMaxTimers Then
        mintMaxTimers = mintMaxTimers + MAXTIMERINCREMEMT
        ReDim Preserve maxti(1 To mintMaxTimers)
    End If
    '
    ' Enregistre une r�f�rence � utiliser lors du d�clenchement
    '   de l'�v�nement Tick de l'objet XTimer.
    Set maxti(intTimerNumber).xt = xt
    '
    ' Enregistre l'ID de la minuterie renvoy�e par l'API
    '   SetTimer et renvoie la valeur � l'objet XTimer.
    maxti(intTimerNumber).id = lngTimerID
    maxti(intTimerNumber).blnReentered = False
    BeginTimer = lngTimerID
End Function

' TimerProc est la proc�dure de minuterie que le syst�me
' ---------   appelle si une de vos minuteries s'arr�te.
'
' IMPORTANT -- �tant donn� que cette proc�dure doit se
'   trouver dans un module standard, tous vos objets Timer
'   doivent la partager. Cela signifie que la proc�dure doit
'   identifier quelle minuterie s'est arr�t�e. Cela est
'   r�alisable en recherchant dans le tableau maxti
'   l'ID de la minuterie (idEvent).
'
' Si cette d�claration de proc�dure est fausse, une
'   ERREUR DE PROGRAMME se produira! Ceci constitue l'un des
'   dangers qui existent lorsque des API qui n�cessitent
'   des fonctions Callback sont utilis�es.
'
Public Sub TimerProc(ByVal hWnd As Long, ByVal uMsg As Long, ByVal idEvent As Long, ByVal lngSysTime As Long)
    Dim intCt As Integer

    For intCt = 1 To mintMaxTimers
        If maxti(intCt).id = idEvent Then
            ' Ne d�clenche pas d'�v�nement si une ancienne
            '   instance de cet �v�nement est en cours de
            '   traitement.
            If maxti(intCt).blnReentered Then Exit Sub
            ' L'indicateur blnReentered bloque les
            '   instances suivantes de cet �v�nement
            '   jusqu'� ce que l'instance en cours se
            '   termine.
            maxti(intCt).blnReentered = True
            On Error Resume Next
            ' D�clenche l'�v�nement Tick pour l'objet
            '   XTimer appropri�.
            maxti(intCt).xt.RaiseTick
            If Err.Number <> 0 Then
                ' Si une erreur se produit, l'objet XTimer
                '   se termine sans que sa minuterie ne s'arrete.
                '   La minuterie orpheline est supprim�e, pour �viter
                '   une erreur de protection g�n�rale ult�rieure.
                KillTimer 0, idEvent
                maxti(intCt).id = 0
                '
                ' Lib�re la r�f�rence � l'objet
                '   XTimer.
                Set maxti(intCt).xt = Nothing
            End If
            '
            ' Autorise cet �v�nement � entrer de nouveau
            '   une proc�dure TimerProc.
            maxti(intCt).blnReentered = False
            Exit Sub
        End If
    Next
    ' La ligne qui suit est prot�g�e contre les �checs,
    '   au cas o� un objet Xtimer serait lib�r� sans que
    '   la minuterie du syst�me ne soit supprim�e.
'
    ' L'ex�cution peut s'arr�ter sur cette ligne en
    '   raison d'un bogue connu avec NT 3.51, avec
    '   lequel vous pouvez recevoir un �v�nement
    '   Timer suppl�mentaire APR�S avoir ex�cut�
    '   l'API KillTimer.
    KillTimer 0, idEvent
End Sub

' La proc�dure EndTimer est appel�e par l'objet XTimer
' ------------------   lorsque la valeur False est affect�e
'   � la propri�t� Enabled et qu'un nouvel intervalle de
'   minuterie est requis. Il n'existe aucun moyen de
'   r�initialiser une minuterie de syst�me. La seule
'   solution pour modifier l'intervalle est donc de
'   supprimer la minuterie existante et d'appeler la
'   m�thode BeginTimer pour en d�marrer une nouvelle.
'
Public Sub EndTimer(ByVal xt As XTimer)
    Dim lngTimerID As Long
    Dim intCt As Integer
    
    ' Demande � l'objet XTimer son num�ro TimerID, n�cessaire
    '   pour rechercher dans le tableau le XTIMERINFO correct. Il
    '   est possible de rechercher la r�f�rence m�me de
    '   l'objet XTimer en utilisant l'op�rateur Is qui compare
    '   xt et maxti(intCt).xt. Cette possiblit� n'offre pas
    '   cependant la m�me rapidit�.
    lngTimerID = xt.TimerID
    '
    ' Si l'ID de la minuterie est �gale � z�ro, la fonction EndTimer
    '   a �t� appel� avec une erreur.
    If lngTimerID = 0 Then Exit Sub
    '
    For intCt = 1 To mintMaxTimers
        If maxti(intCt).id = lngTimerID Then
            ' Supprime la minuterie du syst�me.
            KillTimer 0, lngTimerID
            '
            ' Lib�re la r�f�rence � l'objet XTimer.
            Set maxti(intCt).xt = Nothing
            '
            ' R�initialise l'ID en lib�rant un emplacement
            '   pour une nouvelle minuterie active.
            maxti(intCt).id = 0
            Exit Sub
        End If
    Next
End Sub

' La proc�dure Scrub peut �tre consid�r�e comme un dispositif
' ---------------   de s�curit� dans le cas du d�bogage: pour
'   mettre fin � ce projet alors qu'il reste des objets XTimer
'   actifs, appelez cette proc�dure � partir de la fen�tre
'   Ex�cution. Cela appellera l'API KillTimer pour toutes les
'   minuteries du syst�me. Ainsi, l'environnement de d�veloppement
'   peut repasser sans risque en mode Cr�ation.
'
Public Sub Scrub()
    Dim intCt As Integer
    ' Supprime les minuteries actives restantes.
    For intCt = 1 To mintMaxTimers
        If maxti(intCt).id <> 0 Then KillTimer 0, maxti(intCt).id
    Next
End Sub
