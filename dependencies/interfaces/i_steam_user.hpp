struct cSyraID
{
	struct SteamIDComponent_t
	{
		int				m_unAccountID : 32;			// unique account identifier
		unsigned int		m_unAccountInstance : 20;	// dynamic instance ID
		unsigned int		m_EAccountType : 4;			// type of account - can't show as EAccountType, due to signed / unsigned difference
		int			m_EUniverse : 8;	// universe this account belongs to

	}scomp;

	int m_unAll64Bits;
};

class i_steam_user {
public:
	cSyraID get_steam_user_id() {
		using original_fn = cSyraID(__thiscall*)(i_steam_user*);
		return (*(original_fn**)this)[54](this);
	}
};