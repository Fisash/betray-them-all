using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class Effect
{
    public int id;
    public EffectTypeByDuration durationType;

    public int expirationTimeNumber;

    public Effect(int id, EffectTypeByDuration durationType, int expirationTimeNumber)
    {
        this.id = id;
        this.durationType = durationType;
        this.expirationTimeNumber = expirationTimeNumber;
    }
}
public enum EffectTypeByDuration
{
    ExpiringMoves,
    ExpiringDays,
    Permanent
}