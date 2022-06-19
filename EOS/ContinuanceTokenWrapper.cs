using Godot;
using Epic.OnlineServices;

public class ContinuanceTokenWrapper : Reference
{
    public ContinuanceTokenWrapper(ContinuanceToken token)
    {
        _internalToken = token;
    }

    public new string GetClass()
    {
        return "ContinuanceTokenWrapper";
    }

    public override string ToString()
    {
        string tokenString = _internalToken.ToString();
        return "ContinuanceTokenWrapper(" + tokenString + ")";
    }

    public ContinuanceToken _internalToken;
}