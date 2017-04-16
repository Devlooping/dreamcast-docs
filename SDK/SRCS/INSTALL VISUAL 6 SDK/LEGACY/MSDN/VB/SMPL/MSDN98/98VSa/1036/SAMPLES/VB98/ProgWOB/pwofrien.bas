Attribute VB_Name = "modFriends"
Option Explicit
' Constantes pour les fonctions de d�bogage.
Global Const DEBUGTOKEN_DebugID = 1
Global Const DEBUGTOKEN_ClassName = 2

' Type d�fini par l'utilisateur, dont se sert
'   l'exemple pour illust� le type Friend.
Public Type udtDEMO
    intA As Integer
    lngB As Long
    strC As String
End Type

' La fonction timeGetTime est utilis� par
' ----------- l'exemple Implements, et par le
'   code de d�bogage de la dur�e de vie de
'   l'objet dans ce module.
Declare Function timeGetTime Lib "winmm.dll" () As Long

' Stockage pour la collection globale
'   pour la dur�e de vie des objets de d�bogage.
'   Utilis� par les proc�dures DebugInit,
'   DebugTerm et DebugShow (ci-dessous).
Private mcolDebug As New Collection

'         PROC�DURES DE D�BOGAGE
'
' DebugInit
' DebugTerm
' DebugShow
'
' Tous les objets impl�mentent l'interface
'   IDebug et la g�rent en appelant la fonction
'   DebugInit(Me) dans leurs �v�nements
'   Initialize, et la proc�dure DebugTerm Me dans
'   leurs �v�nements Terminate. Vous pouvez
'   utiliser la fonction DebugShow dans la fen�tre Ex�cution
'   pour lister un objet actif, tous les
'   objets actifs, ou tous les objets d'une classe.
'
' Tous les objets de ce projet sont
'   d�finis pour utiliser ces fonctions.
'
' -------------------------------------
' La fonction DebugInit est appel�e par chaque objet,
' ---------     dans son �v�nement Initialize.
'   DebugInit ajoute une cha�ne de d�bogage pour
'   l'objet de la collection globale, et
'   renvoie un DebugID unique pour l'objet.
'   La m�thode peut afficher en option la
'   cha�ne de d�bogage dans la fen�tre Ex�cution
'   (la valeur par d�faut est True).
'
Public Function DebugInit(ByVal obj As Object, _
        Optional ByVal ShowImmediate As Boolean = True) As Long
    Dim lngDebugID As Long
    Dim strDebug As String
    
    ' Obtient un num�ro d'identificateur unique.
    lngDebugID = GetDebugID
    ' La cha�ne de d�bogage de chaque
    '   objet affiche le DebugID, le
    '   nom de classe de l'objet, et l'heure
    '   de d�but (exprim�e en secondes depuis
    '   que le premier objet de d�bogage a �t�
    '   cr��, sous forme de valeur de type Double,
    '   les millisecondes �tant affich�es sous
    '   forme d�cimale).
    strDebug = lngDebugID & " " _
        & TypeName(obj) _
        & " (cr�� � " & DebugTime & ")"
    '
    ' Ajoute la cha�ne � la collection,
    '   en utilisant l'identificateur unique comme une cl�.
    mcolDebug.Add strDebug, CStr(lngDebugID)
    '
    ' L'option par d�faut est d'afficher la cha�ne
    '   de d�bogage dans la fen�tre Ex�cution.
    If ShowImmediate Then Debug.Print strDebug
    '
    ' Renvoie le DebugID. L'objet doit
    '   le stocker comme une partie de
    '   l'impl�mentation de IDebug.
    DebugInit = lngDebugID
End Function

' La proc�dure DebugTerm est appel� par chaque objet,
' ---------     dans son �v�nement Terminate.
'   La proc�dure DebugTerm supprime la cha�ne de d�bogage
'   de l'objet de la collection globale,
'   et affiche en option (la valeur par d�faut
'   est True) la cha�ne de d�bogage dans
'   la fen�tre Ex�cution.
'
Public Sub DebugTerm(ByVal obj As Object, _
        Optional ByVal ShowImmediate As Boolean = True)
    
    Dim idbg As IDebug
    
    On Error Resume Next
    '
    ' Obtient une r�f�rence � l'interface
    '   IDebug de l'objet.
    Set idbg = obj
    If Err.Number <> 0 Then
        MsgBox TypeName(obj) & " n'impl�mente pas IDebug; impossible d'enregistrer la fin.", , "DebugTerm"
        Exit Sub
    End If
    '
    ' La valeur par d�faut est d'afficher la
    '   cha�ne de d�bogage dans la Fen�tre Ex�cution.
    If ShowImmediate Then Debug.Print _
        mcolDebug(CStr(idbg.DebugID)) _
        & " (Termin� � " & DebugTime & ")"
    '
    ' Supprime la cha�ne de la collection.
    ' 
    mcolDebug.Remove CStr(idbg.DebugID)
End Sub

' La proc�dure DebugShow affiche la ou les cha�nes
' ---------     de d�bogage pour toute la liste
'   des objets actifs, pour tous les objets actifs
'   d'une classe, ou pour un objet particulier.
'   Appellez la proc�dure DebugShow � partir de la fen�tre
'   Ex�cution sans argument (affiche tout),
'   avec un nom de classe (affiche toute la classe),
'   avec une r�f�rence d'objet (affiche cet
'   objet), ou le DebugID d'un objet
'   (affiche cet objet).
'
Public Sub DebugShow(Optional ByVal What As Variant)
    Dim vnt As Variant
    Dim idbg As IDebug
    
    On Error GoTo NoShow
    ' Si aucun argument n'est fourni, affiche
    '   tous les objets actifs. (Il peut �tre
    '   utile d'avoir un deuxi�me param�tre optionnel
    '   FileName qui vous permettrait
    '   de les enregistrer vers un fichier, o� m�me
    '   dans le Presse-papiers.)
    If IsMissing(What) Then
        What = "<Tout>"
        For Each vnt In mcolDebug
            Debug.Print vnt
        Next
    '
    ' Si un objet est fourni, utilise son
    '   DebugID pour rechercher sa cha�ne de
    '   d�bogage.
    ElseIf IsObject(What) Then
        On Error Resume Next
        '
        ' Obtient une r�f�rence � l'interface
        '   IDebug de l'objet.
Set idbg = What
        If Err.Number <> 0 Then
            MsgBox TypeName(What) & " n'impl�mente pas IDebug; impossible d'afficher l'enregistrement du d�bogage.", , "DebugShow"
            Exit Sub
        End If
        '
        Debug.Print mcolDebug(CStr(idbg.DebugID))
    '
    ' Si un nombre est fourni, consid�re qu'il s'agit
    '   du DebugID et l'utilise pour rechercher la cha�ne.
    ' 
    ElseIf IsNumeric(What) Then
        Debug.Print mcolDebug(CStr(What))
    '
    ' Si ce n'est pas un nombre, consid�re qu'il
    '   s'agit d'une cha�ne contenant le nom de
    '   la classe; affiche tous les objets avec
    '   le nom de la classe.
    Else
        For Each vnt In mcolDebug
            If What = GetDebugToken(vnt, DEBUGTOKEN_ClassName) Then
                Debug.Print vnt
            End If
        Next
    End If
    Exit Sub
    
NoShow:
    If IsObject(What) Then
        MsgBox "Impossible d'afficher des informations. Est-ce que cet objet a �t� d�fini pour le d�bogage?", , "DebugShow"
    Else
        MsgBox "Impossible d'afficher des informations pour " _
            & What & ". Est-ce que cet objet a �t� d�fini pour le d�bogage?", , "DebugShow"
    End If
End Sub

' La fonction GetDebugString renvoie une cha�ne
' --------------    de l'objet depuis la collection
'   globale.
'
Public Function GetDebugString(ByVal obj As Object) As String
    Dim idbg As IDebug
    
    On Error Resume Next
    '
    ' Obtient une r�f�rence � l'interface
    '   IDebug de l'objet.
    Set idbg = obj
    GetDebugString = mcolDebug(CStr(idbg.DebugID))
End Function

' La fonction GetDebugID est utilis�e pour affecter � chaque objet
' ----------    un num�ro d'identificateur unique, �
'   des fins de d�bogage.
Public Function GetDebugID() As Long
    Static lngLastID As Long
    lngLastID = lngLastID + 1
    GetDebugID = lngLastID
End Function

' La fonction GetDebugToken analyse la cha�ne de d�bogage
' -------------     pour un objet et
'   renvoie le jeton demand�. Les jetons
'   sont s�par�s par des espaces simples.
'   (1) DebugID
'   (2) Nom de la classe
'
' Il existe d'autres jetons, mais ils n'ont
'   pas le bon type.
'
Public Function GetDebugToken( _
        ByVal DebugString As String, _
        ByVal TokenNumber As Integer) As String

    Dim inx1 As Long
    Dim inx2 As Long
    Dim ct As Integer
    
    If TokenNumber <= 0 Then
        Err.Raise vbObjectError + 1060, , _
            "Mauvais num�ro de jeton dans la fonction GetDebugToken"
    Else
        inx2 = 1
        For ct = 1 To TokenNumber
            inx1 = inx2
            inx2 = InStr(inx1, DebugString, " ")
            If inx2 = 0 Then Exit For
        Next
        If inx2 = 0 Then
            GetDebugToken = ""
        Else
            GetDebugToken = Mid$(DebugString, inx1 + 1, inx2 - inx1)
        End If
    End If
End Function
        
' La fonction DebugTime utilise l'API timeGetTime
' ---------     pour compter les millisecondes
'   �coul�es depuis le d�marrage de l'ordinateur.
'   Ce nombre est converti en une valeur de type Double
'   contenant le nombre de secondes depuis la cr�ation
'   du premier objet de d�bogage (s,mmm), en utilisant
'   comme heure de base la premi�re instance o� cette
'   fonction a �t� appel�e. Avec cette fonction, les
'   valeurs d'heure sont plus faciles � utiliser qu'un
'   nombre brut exprim� en millisecondes, compt�es depuis
'   le dernier d�marrage de la machine, dans la mesure o�
'   (1) il peut s'agir d'un nombre tr�s long, et (2) qu'il
'   peut �tre n�gatif, comme cela est expliqu� plus loin.
'
Public Function DebugTime() As Double
    Static timeBase As Double
    Dim timeCurrent As Double
    
    If timeBase = 0 Then
        ' Initialise l'heure de base. La boucle
        '   tient compte du fait que l'heure
        '   renvoy�e par la fonction timeGetTime
        '   peut repasser par z�ro, si l'ordinateur
        '   reste allum� suffisamment longtemps.
        Do While timeBase = 0
            timeBase = timeGetTime
        Loop
        '
        ' La valeur renvoy�e par la fonction timeGetTime
        '   peut �tre n�gative (reportez-vous � la note
        '   ci-dessous) si l'ordinateur est rest�
        '   longtemps allum�.
        '   Correct pour cela.
        If timeBase < 0 Then
            timeBase = timeBase + 4294967296#
        End If
    End If
    '
    timeCurrent = timeGetTime
    '
    ' Correct pour la valeur n�gative,
    '   si n�cessaire.
    If timeCurrent < 0 Then
        timeCurrent = timeCurrent + 4294967296#
    End If
    '
    ' G�re le cas o� la fonction timeGetTime
    '   repasse par z�ro.
    If timeCurrent < timeBase Then
        DebugTime = (timeCurrent + 4294967296# - timeBase) / 1000#
    Else
        DebugTime = (timeCurrent - timeBase) / 1000#
    End If
End Function
' -------- Fonction timeGetTime --------
' Les millisecondes �coul�es depuis le dernier
'   d�marrage sont exprim�es sous forme d'un
'   entier binaire de 4 octets non sign�, ce
'   qui signifie qu'il peut �tre plus long que
'   la valeur de type Long support�e.
'   Lorsqu'il passe le plus grand nombre positif
'   g�r� par une valeur de type Long (soit 2147483647),
'   Visual Basic consid�re que le nombre a "fait
'   le tour" et le transforme en nombre n�gatif.
'   Si cela se produit, l'incr�mentation se poursuit
'   jusqu'� atteindre le plus grand nombre n�gatif
'   g�r� par la valeur de type Long, jusqu'� z�ro,
'   et ainsi de suite.

' Cela cr�� un mod�le en 'dents de scie',
'   et fonctionne assez bien pour les
'   diff�rences d'heures (ce que la fonction
'   DebugTime calcule), sauf au moment critique
'   o� la remise � z�ro se produit.
'
' La fonction DebugTime r�sout ce probl�me
'   en pla�ant le nombre dans un conteneur
'   plus large, de type Double. Si le
'   nombre est n�gatif, il peut �tre converti
'   dans le nombre qu'il aurait d� �tre
'   en lui ajoutant 4294967296.
