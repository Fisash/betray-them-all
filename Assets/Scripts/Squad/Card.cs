using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public class Card : ISaleable
{
    public string name;

    public Class type;

    public int iconID;
    public int level;

    public int exp;
    public int maxExp;


    public int addedExp;
    public int hp;
    public int maxHp;
    public int addingMaxHp;

    public int baseDamage;
    public int baseDamageOffset;
    public int addingDamage;

    public int baseCriticalStrikeChance;
    public int addingCriticalStrikeChance;

    public int baseProtection;
    public int baseProtectionOffset;
    public int addingProtection;

    public int baseEvasionChance;
    public int addingEvasionChance;

    public Weapon weapon;
    public Armor armor;

    public bool isNotEmpty;
    public int price;

    public Card leader = null;

    public int Price => price + weapon.Price + armor.Price;

    public List<Effect> effects = new();

    public void AddExp(int exp)
    {
        this.exp += exp;
        if (this.exp >= maxExp)
        {
            int remainingExp = this.exp - maxExp;
            maxExp = Mathf.RoundToInt(maxExp * 1.4f);
            LevelUp();
            this.exp = 0;
            if (remainingExp > 0)
            {
                AddExp(remainingExp);
            }
        }
    }
    public void LevelUp()
    {
        level += 1;
        maxHp += 5;
        baseDamage += 2;
        baseDamageOffset += 1;
        baseProtection += 1;
        baseProtectionOffset += 1;
        baseEvasionChance += 2;
        baseCriticalStrikeChance += 1;
    }

    public bool HaveIEffect(int id)
    {
        foreach(Effect effect in effects)
        {
            if (effect.id == id)
                return true;
        }
        return false;
    }
    public void RemoveEffect(Effect effect, ItemsAndClassesInfo info)
    {
        effects.Remove(effect);
        EffectInfo effInfo = info.effects[effect.id];

        foreach (EffectAction action in effInfo.actions)
        {
            switch (action.type)
            {
                case EffectActopnType.ChangeMaxHp:
                    addingMaxHp -= action.effectPower;
                    break;
                case EffectActopnType.ChangeProtection:
                    addingProtection -= action.effectPower;
                    break;
                case EffectActopnType.ChangeDamage:
                    addingDamage -= action.effectPower;
                    break;
                case EffectActopnType.ChangeEvasionChance:
                    addingEvasionChance -= action.effectPower;
                    break;
                case EffectActopnType.ChangeCriticalStrikeChance:
                    addingCriticalStrikeChance -= action.effectPower;
                    break;
            }
        }
    }
    public void ApplyEffect(Effect effect, ItemsAndClassesInfo info)
    {
        foreach (Effect eff in effects)
        {
           if (eff.id == effect.id)
           {
                eff.expirationTimeNumber += effect.expirationTimeNumber;
                return;
           }
        }
        
        effects.Add(effect);
        EffectInfo effInfo = info.effects[effect.id];

        foreach(EffectAction action in effInfo.actions)
        {
            switch (action.type)
            {
                case EffectActopnType.ChangeMaxHp:
                    addingMaxHp += action.effectPower;
                    if(hp > maxHp + addingMaxHp)
                    {
                        hp = maxHp + addingMaxHp;
                    }
                    break;
                case EffectActopnType.ChangeProtection:
                    addingProtection += action.effectPower;
                    break;
                case EffectActopnType.ChangeDamage:
                    addingDamage += action.effectPower;
                    break;
                case EffectActopnType.ChangeEvasionChance:
                    addingEvasionChance += action.effectPower;
                    break;
                case EffectActopnType.ChangeCriticalStrikeChance:
                    addingCriticalStrikeChance += action.effectPower;
                    break;
            }
        }

    }
    public int GetEvasionChance()
    {
        int result = baseEvasionChance + addingEvasionChance;
        if (armor != null)
        {
            result += armor.evasionChance;
        }
        if (result < 0) result = 0;
        return result;
    }
    public int GetCriticalStikeChance()
    {
        int result = baseCriticalStrikeChance + addingCriticalStrikeChance;
        if (weapon != null) 
        {
            result += weapon.criticalStrikeChance;
        }
        if (result < 0) result = 0;
        return result;
    }
    public Vector2Int GetProtection()
    {
        int maxProtection = baseProtection + addingProtection;
        if (armor != null)
        {
            maxProtection += armor.protection;
        }
        int minProtection = maxProtection - baseProtectionOffset;
        if (minProtection < 0) minProtection = 0;
        if (maxProtection < 0) maxProtection = 0;
        return new Vector2Int(minProtection, maxProtection);
    }

    public Vector2Int GetDamage()
    {
        int maxDamage = baseDamage + addingDamage;
        if (weapon != null)
        {
            maxDamage += weapon.damage;
        }
        int minDamage = maxDamage - baseDamageOffset;
        if (minDamage < 0)
            minDamage = 0;
        if (maxDamage < 0)
            maxDamage = 0;
        return new Vector2Int(minDamage, maxDamage);
    }
    public string GetTakingHitResult(int damage)
    {

        if (Random.Range(1, 101) <= GetEvasionChance())
        {
            return name + " уклонился от атаки";
        }
        int damageWithProtection = damage - Random.Range(GetProtection().x, GetProtection().y+1);
        if(damageWithProtection < 0)
        {
            damageWithProtection = 0;
        }
        hp -= damageWithProtection;
        return name + " получил " + damageWithProtection.ToString() + " урона";
    }

    public string GetTakingHillResult(int hill)
    {
        hp += hill;
        if(hp > maxHp + addingMaxHp)
        {
            hp = maxHp + addingMaxHp;
        }
        return name + " восстановил " + hill.ToString() + " здоровья";
    }

    public int GetRandNormalDamage()
    {
        return Random.Range(GetDamage().x, GetDamage().y+1);
    }

    public bool IsCriticalStrikeAtack()
    {
        return Random.Range(1, 101) <= GetCriticalStikeChance();
    }
    public Card(int classID, ItemsAndClassesInfo info, bool isEnemy = false, int startLevel = 1)
    {
        isNotEmpty = true;
        level = 1;
        exp = 0;
        maxExp = 100;
        ClassInfo classInfo = isEnemy ? info.enemyPatterns[classID] : info.classes[classID];
        price = Random.Range(classInfo.minPrice, classInfo.maxPrice+1);
        maxHp = Random.Range(classInfo.minStartHp, classInfo.maxStartHp+1);
        hp = maxHp;
        baseDamage = Random.Range(classInfo.minStartBaseDamage, classInfo.maxStartBaseDamage+1);
        baseDamageOffset = Random.Range(classInfo.minBaseDamageOffset, classInfo.maxBaseDamageOffset+1);
        baseCriticalStrikeChance = Random.Range(classInfo.minBaseCriticalStrikeChance, classInfo.maxBaseCriticalStrikeChance+1);
        baseProtection = Random.Range(classInfo.minBaseProtection, classInfo.maxBaseProtection+1);
        baseProtectionOffset = Random.Range(classInfo.minBaseProtectionOffset, classInfo.maxBaseProtectionOffset+1);
        baseEvasionChance = Random.Range(classInfo.minBaseEvasionChance, classInfo.maxBaseEvasionChance+1);
        type = classInfo.mercenaryClass;
        iconID = Random.Range(0, classInfo.icnons.Length);

        WeaponType[] suiteWeapons = classInfo.weaponsOnStart;
        if (suiteWeapons.Length > 0)
        {
            WeaponType wType = suiteWeapons[Random.Range(0, suiteWeapons.Length)];
            WeaponPattern weaponPattern = info.GetWeaponPatternByEnam(wType);
            Weapon weapon = new Weapon(info.GetPatternID(weaponPattern), weaponPattern);
            this.weapon = weapon;
        }

        ArmorType[] suiteArmors = classInfo.armorOnStart;
        if (suiteArmors.Length > 0)
        {
            ArmorType aType = suiteArmors[Random.Range(0, suiteArmors.Length)];
            ArmorPattern armorPattern = info.GetArmorPatternByEnam(aType);
            Armor armor = new Armor(info.GetPatternID(armorPattern), armorPattern);
            this.armor = armor;
        }

        if (isEnemy)
        {
            name = classInfo.name;
        }
        else
        {
            string[] firstName = new string[] { "Рол", "Боб", "Буб", "Вас", "Ваг", "Раг", "Рул", "Гал", "Гат", "Гел", "Гул", "Мел", "Мул", "Тамп", "Тенд", "Тонд", "Ул", "Ел", "Кал", "Алд" };
            string[] secondName = new string[] { "граф", "дон", "фрик", "с", "ор", "ил", "ел", "иер", "ивер", "рик", "адор", "уин" };

            name = firstName[Random.Range(0, firstName.Length)] + secondName[Random.Range(0, secondName.Length)];
        }

        if(startLevel != 1)
        {
            while(level != startLevel)
            {
                AddExp(100);
            }
            hp = maxHp;
        }
    }
}
[System.Serializable]
public class Item : ISaleable
{
    public int price;
    public virtual int Price => price;

    public int patternID;
    public Item(int patternID, int price)
    {
        this.patternID = patternID;
        this.price = price;
    }
}

[System.Serializable]
public class Hill : Item
{
    public int minHilledHp;
    public int maxHilledHp;

    public Hill(int patternID, int price, int minHilledHp, int maxHilledHp) : base(patternID, price)
    {
        this.minHilledHp = minHilledHp;
        this.maxHilledHp = maxHilledHp;
    }
    public Hill(int patternID, HillPattern pattern) : base(patternID, pattern.cost)
    {
        minHilledHp = pattern.minHilledHp;
        maxHilledHp = pattern.maxHilledHp;
    }
}

[System.Serializable]
public class Weapon : Item
{
    public int damage;
    public int criticalStrikeChance;
    public WeaponType type;

    public override int Price => base.Price + damage + criticalStrikeChance; 

    public Weapon(int patternID, int price, int damage, int criticalStrikeChance, WeaponType type) : base(patternID, price)
    {
        this.criticalStrikeChance = criticalStrikeChance;
        this.damage = damage;
        this.type = type;
    }

    public Weapon(int patternID,  WeaponPattern pattern) : base(patternID, pattern.cost)
    {
        damage = Random.Range(pattern.minStartDamage, pattern.maxStartDamage + 1);
        criticalStrikeChance = Random.Range(pattern.minStartCriticalChance, pattern.maxStartCriticalChance + 1);
        type = pattern.type;
    }
}

[System.Serializable]
public enum WeaponType
{
    Sword,
    Axe,
    Spear,
    Hammer,
    Bow,
    Crossbow,
    Dagger,
    Stick
}

[System.Serializable]
public class Armor : Item
{
    public int protection;
    public int evasionChance;
    public ArmorType type;

    public override int Price => base.Price + protection + evasionChance;

    public Armor(int patternID, int price, int protection, int evasionChance, ArmorType type) : base(patternID, price)
    {
        this.protection = protection;
        this.evasionChance = evasionChance;
        this.type = type;
    }

    public Armor(int patternID, ArmorPattern pattern) : base(patternID, pattern.cost)
    {
        protection =  Random.Range(pattern.minStartProtection, pattern.maxStartProtection + 1);
        evasionChance = Random.Range(pattern.minStartEvasionChance, pattern.maxStartEvasionChance + 1);
        type = pattern.type;
    }
}

[System.Serializable]
public enum ArmorType
{
    ChainMail,
    Plate,
    Fabric
}

[System.Serializable]
public enum Class
{
    Warrior,
    Tank,
    Outlaw,
    Archer,
    Healer,
    Bear,
    Wolf,
    Test,
    OutlawLeader,
    OutlawBase,
    OutlawArcher,
    Harpy,
    FireSpirit,
    SpiritMinion,
    Vampire,
    Ghoul, 
    Skeleton,
    Troll,
    Griffin,
    Lich
}