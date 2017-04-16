Attribute VB_Name = "modDHTML"
' Le code suivant vous permet d'utiliser le jeu de propri�t�s
' du navigateur Web pour conserver des informations entre
' diff�rentes pages HTML.
Public objWebBrowser As WebBrowser

' PutProperty: Stocke des informations dans le jeu de propri�t�s en appelant cette
'              fonction.  Les param�tres d'entr�e sont la propri�t� nomm�e
'              et la valeur qu'elle doit stocker.
'
Public Sub PutProperty(strName As String, vntValue As Variant)
    
    ' V�rifier s'il existe une instance du navigateur.
    If objWebBrowser Is Nothing Then Set objWebBrowser = New WebBrowser
    
    ' Appeler la m�thode PutProperty du navigateur pour stocker la valeur.
    objWebBrowser.PutProperty strName, vntValue

End Sub

' GetProperty: Extraire des informations du jeu de propri�t�s en appelant cette
'              fonction.  Le param�tre d'entr�e est la propri�t� nomm�e
'              et la valeur de retour de la fonction est la valeur actuelle
'              de la propri�t�.
'
Public Function GetProperty(strName As String) As Variant
    
    ' V�rifier s'il existe une instance du navigateur.
    If objWebBrowser Is Nothing Then Set objWebBrowser = New WebBrowser
    
    ' Appeler la m�thode GetProperty du navigateur pour lire la valeur.
    GetProperty = objWebBrowser.GetProperty(strName)

End Function
