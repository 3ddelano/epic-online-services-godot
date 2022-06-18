using Godot;
using Epic.OnlineServices;

public class ContinuanceTokenWrapper : Reference
{
    public ContinuanceTokenWrapper(ContinuanceToken token)
    {
        _internalToken = token;
    }

    public string GetClass()
    {
        return "ContinuanceTokenWrapper";
    }

    public override string ToString()
    {
        return "ContinuanceTokenWrapper(" + _internalToken.ToString() + ")";
    }

    public ContinuanceToken _internalToken;
}