using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(menuName = "Custom/Skill")]

public class Skill : ScriptableObject
{
    public string skillName;
    public Sprite icon;
    public string descriprion;
    public int id;
    public int requiredLevel;
    public int baseGivenExp;

    public AudioClip[] sound;

    public TypeByTarget typeByTarget;
    public TypeByAttackerPosition atackerPos;
    public EnemyStrategy enemyStrategy;

    public List<ApplyingEffect> effects;
}

[System.Serializable]
public struct ApplyingEffect
{
    public Effect effect;
    public int applyChance;
}
public enum TypeByTarget
{
    OneEnemyNearLine,
    OneEnemy,
    AllEnemyes,
    OneAlly,
    AllAlly,
    Self
}

public enum EnemyStrategy
{
    Random,
    ChooseWithLessHP
}

public enum TypeByAttackerPosition
{
    AnyPos,
    OnlyFirstLine
}